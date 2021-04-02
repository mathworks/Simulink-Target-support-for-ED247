classdef Pipeline < matlab.mixin.SetGet
    
    %% DEPENDENT PROPERTIES
    properties (Dependent)
        Configuration
        CoverageFile
        Mode
        ReportFile
        Results
        RootFolder
        Stages
        Status
        TAPFile
    end
    
    %% PRIVATE PROPERTIES
    properties (Access = private)
        coveragefile_
        mode_
        reportfile_
        results_
        stages_
        status_
        tapfile_
    end
    
    %% IMMUTABLE PROPERTIES
    properties (SetAccess = immutable, GetAccess = private)
        configuration_
        project_
    end
    
    %% CONSTRUCTOR
    methods
        
        function obj = Pipeline(rootfolder,varargin)
            
            proj = slproject.getCurrentProjects();
            if isempty(proj)
                obj.project_ = simulinkproject(rootfolder);
            else
                obj.project_ = proj;
            end
            
            configuration = ed247.Configuration.forFolder(obj.project_.RootFolder);
            obj.configuration_  = configuration.toStruct();
            
            obj.mode_           = 'prod';
            obj.coveragefile_   = fullfile(obj.project_.RootFolder,sprintf('CoverageResults-r%s.xml',version('-release')));
            obj.reportfile_     = fullfile(obj.project_.RootFolder,sprintf('TestReport-r%s.pdf',version('-release')));
            obj.tapfile_        = fullfile(obj.project_.RootFolder,sprintf('TAPResults-r%s.tap',version('-release')));
            
            obj.stages_ = {'package';'install';'test';'cleanup'};
            
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
        
        function stages = get.Stages(obj)
            stages = obj.stages_;
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
        
        function set.Stages(obj,stages)
            obj.stages_ = stages;
        end
        
        function set.TAPFile(obj,tapfile)
            obj.tapfile_ = tapfile;
        end
        
    end
    
    %% PUBLIC METHODS
    methods
        
        function varargout = run(obj)
            
            obj.status_ = 0;
            
            try
                
                if ismember('cleanup',obj.stages_)
                    c = onCleanup(@() cleanup(obj));
                end
                
                if ismember('package',obj.stages_)
                    package(obj)
                end
                                                
                if ismember('install',obj.stages_)
                    install(obj)
                end
                
                if strcmp(obj.mode_,'prod') && ismember('test',obj.stages_)
                    close(obj.project_)
                    addpath(fullfile(obj.project_.RootFolder,'tests'))                    
                end
                
                if ismember('test',obj.stages_)
                    [obj.status_,obj.results_] = test(obj);
                end
                                                
            catch me
                obj.show(me.getReport())
                obj.status_ = -1;
            end
            
            if nargout
                varargout = {obj.status_};
            end
            
        end
        
    end
    
    %% PROTECTED METHODS
    methods (Access = protected)
        
        function package(obj)
            
            copyfile(obj.configuration_.Filename, [obj.configuration_.Filename,'.bckp'])
            resetMetadata = onCleanup(@() movefile([obj.configuration_.Filename,'.bckp'],obj.configuration_.Filename));
            
            obj.print( '## Reset .metadata file ("%s")\n', obj.configuration_.Filename);
            configuration = ed247.Configuration.fromStruct(obj.configuration_);
            reset(configuration)
            
            %
            % Package toolbox
            %
            toolboxproject = fullfile(obj.project_.RootFolder,'ToolboxPackagingConfiguration.prj');
            toolboxfile = fullfile(obj.project_.RootFolder, sprintf('ED247_for_Simulink-r%s.mltbx', version('-release')));
            
            obj.print( '## Package toolbox into "%s"\n', toolboxfile);
            matlab.addons.toolbox.packageToolbox(toolboxproject, toolboxfile)
            
        end
        
        function install(obj)
            
            libxml2folder = obj.configuration_.LibXML2;
            if ~isempty(libxml2folder)
                
                if ~isdir(libxml2folder) %#ok<ISDIR> Backward compatibility
                    obj.print( '## Create folder "%s"\n', libxml2folder);
                    mkdir(libxml2folder)
                end
                
                lixml2archive = fullfile(obj.project_.RootFolder,'tests','_files','libxml2.zip');
                obj.print( '## Extract LibXML2 in folder "%s"\n', lixml2archive);
                unzip(lixml2archive,libxml2folder)
                
            end
            
            ed247folder = obj.configuration_.ED247;
            if ~isempty(ed247folder)
                
                if ~isdir(ed247folder) %#ok<ISDIR> Backward compatibility
                    obj.print( '## Create folder "%s"\n', ed247folder);
                    mkdir(ed247folder)
                end
                
                ed247archive = fullfile(obj.project_.RootFolder,'tests','_files','ed247.zip');
                obj.print( '## Extract ED247 in folder "%s"\n', ed247folder);
                unzip(ed247archive,ed247folder)
                
            end
            
            toolboxfile = fullfile(obj.project_.RootFolder, sprintf('ED247_for_Simulink-r%s.mltbx', version('-release')));
            obj.print( '## Install toolbox ("%s")\n', toolboxfile);
            matlab.addons.toolbox.installToolbox(toolboxfile);
            
        end
        
        function [status,results] = test(obj,varargin)
            
            obj.print( '## Create test suite\n');
            ts = matlab.unittest.TestSuite.fromPackage('ed247', 'IncludeSubpackages', true);
            
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
            if ismember(obj.mode_,{'ci','prod'})
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
            status = nnz([results.Failed]);
            
        end
        
        function cleanup(obj)
            
            libxml2folder = obj.configuration_.LibXML2;
            if ~isempty(libxml2folder) && ~isdir(libxml2folder) %#ok<ISDIR> Backward compatibility
                obj.print( '## Delete folder "%s"\n', libxml2folder);
                rmdir(libxml2folder,'s')
            end
            
            ed247folder = obj.configuration_.ED247;
            if ~isempty(ed247folder) && ~isdir(ed247folder) %#ok<ISDIR> Backward compatibility
                obj.print( '## Delete folder "%s"\n', ed247folder);
                rmdir(ed247folder,'s')
            end
            
            toolboxes = matlab.addons.toolbox.installedToolboxes();
            if ~isempty(toolboxes) && any(strcmp({toolboxes.Name},'ED247_for_Simulink'))
                tlbx = toolboxes(strcmp({toolboxes.Name},'ED247_for_Simulink'));
                obj.print( '## Uninstall toolbox ("%s")\n', tlbx.Name);
                matlab.addons.toolbox.uninstallToolbox(tlbx);
            end
            
            if ~isLoaded(obj.project_)
                obj.print('## Re-Open project');
                testfolder = fullfile(obj.project_.RootFolder,'tests');
                if contains(path,testfolder)
                    rmpath(testfolder)
                end
                simulinkproject(obj.project_.RootFolder);
            end
            
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