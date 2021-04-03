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
            exit(obj.run())
        end
        
        function varargout = run(obj)
            
            obj.status_ = 0;
            obj.results_ = matlab.unittest.TestResult.empty;
            
            try
                
                installDependencies(obj)                
                
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