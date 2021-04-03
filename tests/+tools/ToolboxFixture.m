classdef ToolboxFixture < matlab.unittest.fixtures.Fixture
    
    %% IMMUTABLE PROPERTIES
    properties (SetAccess=immutable,GetAccess=private)
        configuration_
        project_
    end
    
    %% CONSTRUCTOR
    methods
        
        function fixture = ToolboxFixture()
            fixture.configuration_ = ed247.Configuration.default();
            fixture.project_ = simulinkproject();
        end
        
    end
    
    %% PUBLIC METHODS
    methods
        
        function setup(fixture)
            
            %
            % Compile MEX
            %
            fixture.print('## Compile ED247 S-Function\n')
            ed247.compile()
            
            %
            % Create default metadata file for packaging (remove
            % user-specific information: MinGW location, ED247 and LibXML2
            % folders)
            %
            copyfile(fixture.configuration_.Filename, [fixture.configuration_.Filename,'.bckp'])
            resetMetadata = onCleanup(@() movefile([fixture.configuration_.Filename,'.bckp'],fixture.configuration_.Filename));
            fixture.print('## Reset .metadata file ("%s")\n', fixture.configuration_.Filename);
            reset(fixture.configuration_)
                                    
            %
            % Package toolbox
            %
            toolboxproject = fullfile(fixture.project_.RootFolder,'ToolboxPackagingConfiguration.prj');
            toolboxfile = fullfile(fixture.project_.RootFolder, sprintf('ED247_for_Simulink-r%s.mltbx', version('-release')));
            
            %
            % Patch
            %   Toolbox project was created in 2016b and the source path is
            %   hard-code in .prj which make it failed in another location
            %
            txt = fileread(toolboxproject);
            txt = regexprep(txt,'C:.*?\ed247_for_simulink',regexptranslate('escape',pwd));
            fid = fopen(toolboxproject,'wt');fprintf(fid,'%s',txt);fclose(fid);
            pause(1) % Pause to ensure that MATLAB path is updated
            fixture.print( '## Package toolbox into "%s"\n', toolboxfile);
            matlab.addons.toolbox.packageToolbox(toolboxproject, toolboxfile)
            
            %
            % Setup Add-Ons installation folder
            %
            userprofile = getenv('USERPROFILE');
            if ~isempty(userprofile)
                s = settings;
                s.matlab.addons.InstallationFolder.PersonalValue = userprofile;
            end
            
            %
            % Install toolbox
            %
            toolboxfile = fullfile(fixture.project_.RootFolder, sprintf('ED247_for_Simulink-r%s.mltbx', version('-release')));
            fixture.print( '## Install toolbox ("%s")\n', toolboxfile);
            matlab.addons.toolbox.installToolbox(toolboxfile);
            
        end
        
        function teardown(fixture)
            
            libxml2folder = fixture.configuration_.LibXML2;
            if ~isempty(libxml2folder) && ~isdir(libxml2folder) %#ok<ISDIR> Backward compatibility
                fixture.print( '## Delete folder "%s"\n', libxml2folder);
                rmdir(libxml2folder,'s')
            end
            
            ed247folder = fixture.configuration_.ED247;
            if ~isempty(ed247folder) && ~isdir(ed247folder) %#ok<ISDIR> Backward compatibility
                fixture.print( '## Delete folder "%s"\n', ed247folder);
                rmdir(ed247folder,'s')
            end
            
            toolboxes = matlab.addons.toolbox.installedToolboxes();
            if ~isempty(toolboxes) && any(strcmp({toolboxes.Name},'ED247_for_Simulink'))
                tlbx = toolboxes(strcmp({toolboxes.Name},'ED247_for_Simulink'));
                fixture.print( '## Uninstall toolbox ("%s")\n', tlbx.Name);
                matlab.addons.toolbox.uninstallToolbox(tlbx);
            end
            
            if ~isLoaded(fixture.project_)
                fixture.print('## Re-Open project');
                testfolder = fullfile(fixture.project_.RootFolder,'tests');
                if contains(path,testfolder)
                    rmpath(testfolder)
                end
                simulinkproject(fixture.project_.RootFolder);
            end
            
        end
        
    end
    
    %% PROTECTED METHODS
    methods (Access=protected)
        
        function bool = isCompatible(fixture, other)
            bool = strcmp(fixture.project_.RootFolder, other.project_.RootFolder);
        end
        
    end
    
    %% PRIVATE METHODS
    methods (Access = private)
        
        function print(obj,varargin) %#ok<INUSL>
            fprintf(1, varargin{:});
        end
        
    end
    
end