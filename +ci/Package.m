classdef Package < matlab.mixin.SetGet
       
    %% IMMUTABLE PROPERTIES
    properties (SetAccess = immutable, GetAccess = private)
        project_
    end
    
    %% CONSTRUCTOR
    methods
       
        function obj = Package(project,varargin)
           
            obj.project_ = project;
            
            if ~isempty(varargin)
                set(obj,varargin{:})
            end
            
        end
        
    end
    
    %% ACCESSORS
    methods
                        
    end
        
    %% PUBLIC METHODS
    methods       
        
        function package(obj)
            
            configuration = ed247.Configuration.default();
            toolboxVersion = configuration.Version;
            
            %
            % Create default metadata file for packaging (remove
            % user-specific information: MinGW location, ED247 and LibXML2
            % folders)
            %
            metadatafilename = configuration.Filename;
            copyfile(metadatafilename, [metadatafilename,'.bckp'])
            resetMetadatafile = onCleanup(@() movefile([metadatafilename,'.bckp'],metadatafilename));
            obj.print('## Reset .metadata file ("%s")', metadatafilename);
            config = ed247.Configuration.fromStruct(configuration);
            reset(config)
            clear('config')
            
            %
            % Package toolbox
            %
            toolboxproject = fullfile(obj.project_.RootFolder,'ToolboxPackagingConfiguration.prj');
            toolboxfile = fullfile(obj.project_.RootFolder, sprintf('ED247_for_Simulink-r%s.mltbx', version('-release')));
            
            %
            % Patch
            %   Toolbox project was created in 2016b and the source path is
            %   hard-code in .prj which make it failed in another location
            %
            txt = fileread(toolboxproject);
            txt = regexprep(txt,'C:.*?\\ed247_for_simulink',regexptranslate('escape',pwd));
            fid = fopen(toolboxproject,'wt');fprintf(fid,'%s',txt);fclose(fid);
            pause(1) % Pause to ensure that MATLAB path is updated
            
            obj.print( '## Set toolbox version to "%s"', toolboxVersion);
            matlab.addons.toolbox.toolboxVersion(toolboxproject, toolboxVersion);
            
            obj.print( '## Package toolbox into "%s"', toolboxfile);
            matlab.addons.toolbox.packageToolbox(toolboxproject, toolboxfile)
            
        end
        
    end
    
    %% HELPER METHODS (PROTECTED)
    methods (Access = protected)
        
        function assert(obj,varargin) %#ok<INUSL>
            assert(varargin{:})
        end
        
        function error(obj,varargin) %#ok<INUSL>
            error(varargin{:})
        end
        
        function warning(obj,varargin) %#ok<INUSL>
            warning(varargin{:})
        end
        
        function print(obj,varargin) %#ok<INUSL>
            fprintf(1,varargin{1} + "\n",varargin{2:end})
        end
        
        function rethrow(obj,varargin) %#ok<INUSL>
            rethrow(varargin{:})
        end
        
    end
    
    %% STATIC METHODS
    methods (Static)
       
        function runOnGitLabCI(varargin)
            
            status = -1;
            
            try
                
                proj = ci.openProject();
                obj = ci.Package(proj);
                
                package(obj);
                
                status = 0;
                
            catch me
                disp(me.getReport())
            end
            
            quit(status)
            
        end
        
        function varargout = runOnLocalComputer(varargin)
            
            proj = ci.openProject();
                        
            obj = ci.Package(proj);
                        
            if nargout
                varargout = {obj};
            end
            
        end
        
    end
    
end