classdef Pipeline < matlab.mixin.SetGet
    
    %% DEPENDENT PROPERTIES
    properties (Dependent)
        Configuration
        CoverageFile
        Mode
        ReportFile
        Results
        RootFolder
        Status
        TAPFile
    end
    
    %% PRIVATE PROPERTIES
    properties (Access = private)
        coveragefile_
        mode_
        reportfile_
        results_
        status_
        tapfile_
        project_
    end
    
    %% IMMUTABLE PROPERTIES
    properties (SetAccess = immutable, GetAccess = private)
        configuration_
        rootfolder_
    end
    
    %% CONSTRUCTOR
    methods
        
        function obj = Pipeline(rootfolder,varargin)
            
            obj.rootfolder_ = rootfolder;
            
            warning('off','ED247:invalidPath')
            openProject(obj)
            warning('on','ED247:invalidPath')
            
            configuration = ed247.Configuration.forFolder(rootfolder);
            obj.configuration_  = configuration.toStruct();
            
            obj.mode_           = 'prod';
            obj.coveragefile_   = fullfile(rootfolder,sprintf('CoverageResults-r%s.xml',version('-release')));
            obj.reportfile_     = fullfile(rootfolder,sprintf('TestReport-r%s.pdf',version('-release')));
            obj.tapfile_        = fullfile(rootfolder,sprintf('TAPResults-r%s.tap',version('-release')));
            
            if ~isempty(varargin)
                set(obj,varargin{:})
            end
            
        end
        
    end
    
    %% ACCESSORS
    methods
        
        function configuration = get.Configuration(obj)
            configuration = obj.configuration_;
        end
        
        function coveragefile = get.CoverageFile(obj)
            coveragefile = obj.coveragefile_;
        end
        
        function mode = get.Mode(obj)
            mode = obj.mode_;
        end
        
        function reportfile = get.ReportFile(obj)
            reportfile = obj.reportfile_;
        end
        
        function results = get.Results(obj)
            results = obj.results_;
        end
        
        function rootfolder = get.RootFolder(obj)
            rootfolder = obj.project_.RootFolder;
        end
        
        function status = get.Status(obj)
            status = obj.status_;
        end
        
        function tapfile = get.TAPFile(obj)
            tapfile = obj.tapfile_;
        end
        
    end
    
    %% ACCESSORS
    methods
        
        function set.CoverageFile(obj,coveragefile)
            obj.coveragefile_ = coveragefile;
        end
        
        function set.Mode(obj,mode)
            obj.mode_ = mode;
        end
        
        function set.ReportFile(obj,reportfile)
            obj.reportfile_ = reportfile;
        end
        
        function set.TAPFile(obj,tapfile)
            obj.tapfile_ = tapfile;
        end
        
    end
    
    %% PUBLIC METHODS
    methods
        
        function exit(obj)
            status = obj.run();
            quit(status, 'force')
        end
        
        function varargout = run(obj)
            
            obj.status_ = 0;
            obj.results_ = matlab.unittest.TestResult.empty;
            
            try
                
                installDependencies(obj)
                
                openProject(obj,'force')
                
                compile(obj)
                package(obj)
                obj.results_ = test(obj);
                
                obj.status_ = nnz([obj.results_.Failed]);
                obj.show(obj.results_)
                
            catch me
                obj.show(me.getReport())
                obj.status_ = -1;
            end
            
            if nargout
                varargout = {obj.status_};
            end
            
        end
        
    end
    
    %% HIDDEN METHODS (Advanced usage)
    methods (Hidden)
        
        function installDependencies(obj)
            
            mingwfolder = getenv(ed247.Configuration.MINGW_ENVIRONMENT_VARIABLE);
            if ~isempty(mingwfolder)
                
                if ~isdir(mingwfolder) %#ok<ISDIR> Backward compatibility
                    obj.print( '## Create folder "%s"\n', mingwfolder);
                    mkdir(mingwfolder)
                end
                
                if ~isdir(fullfile(mingwfolder,'bin')) %#ok<ISDIR> Backward compatibility
                    mingwarchive = fullfile(obj.project_.RootFolder,'tests','_files','MINGW64_4.9.2-airbus.zip');
                    obj.print( '## Extract MinGW64 in folder "%s"\n', mingwarchive);
                    unzip(mingwarchive,mingwfolder)
                else
                    obj.print( '## MinGW64 is already installed\n');
                end
                
            end
            
            libxml2folder = obj.configuration_.LibXML2;
            if ~isempty(libxml2folder)
                
                if ~isdir(libxml2folder) %#ok<ISDIR> Backward compatibility
                    obj.print( '## Create folder "%s"\n', libxml2folder);
                    mkdir(libxml2folder)
                end
                
                if ~isdir(fullfile(libxml2folder,'lib'))  %#ok<ISDIR> Backward compatibility
                    lixml2archive = fullfile(obj.project_.RootFolder,'tests','_files','libxml2.zip');
                    obj.print( '## Extract LibXML2 in folder "%s"\n', lixml2archive);
                    unzip(lixml2archive,libxml2folder)
                else
                    obj.print( '## LibXML2 is already installed\n');
                end
                
            end
            
            ed247folder = obj.configuration_.ED247;
            if ~isempty(ed247folder)
                
                if ~isdir(ed247folder) %#ok<ISDIR> Backward compatibility
                    obj.print( '## Create folder "%s"\n', ed247folder);
                    mkdir(ed247folder)
                end
                
                if ~isdir(fullfile(ed247folder,'lib'))  %#ok<ISDIR> Backward compatibility
                    ed247archive = fullfile(obj.project_.RootFolder,'tests','_files','ed247.zip');
                    obj.print( '## Extract ED247 in folder "%s"\n', ed247folder);
                    unzip(ed247archive,ed247folder)
                else
                    obj.print( '## ED247 is already installed\n');
                end
                
            end
            
        end
        
        function openProject(obj,varargin)
            
            proj = slproject.getCurrentProjects();
            if isempty(proj) || (~isempty(varargin) && strcmp(varargin{1},'force'))
                obj.project_ = simulinkproject(obj.rootfolder_);
            else
                obj.project_ = proj;
            end
            
        end
        
        function compile(obj)
            
            %
            % Compile MEX
            %
            obj.print('## Compile ED247 S-Function\n')
            ed247.compile('MEXFile', 'ed247_sfun.c')
            
            obj.print('## Compile ED247 C-MEX\n')
            ed247.compile('MEXFile', 'ed247_mex.c')
            
        end
        
        function package(obj)
            
            %
            % Create default metadata file for packaging (remove
            % user-specific information: MinGW location, ED247 and LibXML2
            % folders)
            %
            metadatafilename = obj.configuration_.Filename;
            copyfile(metadatafilename, [metadatafilename,'.bckp'])
            resetMetadatafile = onCleanup(@() movefile([metadatafilename,'.bckp'],metadatafilename));
            obj.print('## Reset .metadata file ("%s")\n', metadatafilename);
            config = ed247.Configuration.fromStruct(obj.configuration_);
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
            txt = regexprep(txt,'C:.*?\ed247_for_simulink',regexptranslate('escape',pwd));
            fid = fopen(toolboxproject,'wt');fprintf(fid,'%s',txt);fclose(fid);
            pause(1) % Pause to ensure that MATLAB path is updated
            obj.print( '## Package toolbox into "%s"\n', toolboxfile);
            matlab.addons.toolbox.packageToolbox(toolboxproject, toolboxfile)
            
        end
        
        function results = test(obj,varargin)
            
            obj.print( '## Create test suite\n');
            ts = [ ...
                matlab.unittest.TestSuite.fromPackage('ed247', 'IncludeSubpackages', true), ...
                matlab.unittest.TestSuite.fromPackage('toolbox', 'IncludeSubpackages', true), ...
                ];
            
            obj.print( '## Create test runner\n');
            tr = matlab.unittest.TestRunner.withTextOutput();
            
            %
            % Coverage
            %
            if ismember(obj.mode_,{'ci','prod'}) && ~verLessThan('MATLAB','9.3') % r2017b
                reportFile = obj.coveragefile_;
                reportFormat = matlab.unittest.plugins.codecoverage.CoberturaFormat(reportFile);
                plugin = matlab.unittest.plugins.CodeCoveragePlugin.forPackage('ed247','Producing',reportFormat);
                obj.print( '## Enable Coverage plugin\n');
                tr.addPlugin(plugin)
            end
            
            %
            % TAP
            %
            if ismember(obj.mode_,{'ci','prod'})
                tapFile = obj.tapfile_;
                if exist(tapFile,'file') == 2
                    obj.print( '## Delete previous TAP report ("%s")\n', tapFile);
                    delete(tapFile)
                end
                tapFile = matlab.unittest.plugins.ToFile(tapFile);
                plugin  = matlab.unittest.plugins.TAPPlugin.producingVersion13(tapFile);
                obj.print( '## Enable TAP plugin\n');
                tr.addPlugin(plugin)
            end
            
            %
            % Report
            %
            if ismember(obj.mode_,{'ci','prod'}) && ~verLessThan('MATLAB','9.2')
                pdfFile = obj.reportfile_;
                plugin = matlab.unittest.plugins.TestReportPlugin.producingPDF(pdfFile);
                obj.print( '## Enable Report plugin\n');
                tr.addPlugin(plugin)
            end
            
            %
            % Run and get results
            %
            results = tr.run(ts);
            obj.show(results)
            
        end
        
    end
    
    %% PRIVATE METHODS
    methods (Access = private)
        
        function print(obj,varargin) %#ok<INUSL>
            fprintf(1, varargin{:});
        end
        
        function show(obj,varargin) %#ok<INUSL>
            disp(varargin{:})
        end
        
    end
    
    %% STATIC METHODS
    methods (Static)
        
        function obj = forFolder(varargin)
            
            if nargin == 1
                rootfolder = varargin{1};
            else
                rootfolder = pwd;
            end
            obj = ci.Pipeline(rootfolder);
            
        end
        
    end
    
end