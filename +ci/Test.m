classdef Test < matlab.mixin.SetGet
    
    %% DEPENDENT PROPERTIES
    properties (Dependent)
        Runner
        TestSuite
    end
    
    %% OPTIONS
    properties
        CoverageEnable      (1,1)   logical     = false;
        CoverageFilename    (1,1)   string      = "TestCoverage.xml";
        JUnitEnable         (1,1)   logical     = false;
        JUnitFilename       (1,1)   string      = "TestResults.xml";
        ReportEnable        (1,1)   logical     = false;
        ReportFilename      (1,1)   string      = "TestResults.pdf";
        ResultFolder        (1,1)   string      = pwd;
        TAPEnable           (1,1)   logical     = false;
        TAPFilename         (1,1)   string      = "TestResults.tap";
    end
    
    %% IMMUTABLE PROPERTIES
    properties (SetAccess = immutable, GetAccess = private)
        project_
    end
    
    %% CONSTRUCTOR
    methods
        
        function obj = Test(project,varargin)
            
            obj.project_ = project;
            
            if ~isempty(varargin)
                set(obj,varargin{:})
            end
            
        end
        
    end
    
    %% ACCESSORS
    methods
        
        function runner = get.Runner(obj)
            
            runner = matlab.unittest.TestRunner.withTextOutput();
            
            %
            % TAP plugin
            %
            if obj.TAPEnable
                tapFile = fullfile(obj.ResultFolder, obj.TAPFilename);
                if exist(tapFile,"file") == 2
                    delete(tapFile)
                end
                plugin = matlab.unittest.plugins.TAPPlugin.producingVersion13(tapFile);
                runner.addPlugin(plugin)
            end
            
            %
            % JUnit plugin
            %
            if obj.JUnitEnable
                xmlFile = fullfile(obj.ResultFolder, obj.JUnitFilename);
                plugin = matlab.unittest.plugins.XMLPlugin.producingJUnitFormat(xmlFile);
                runner.addPlugin(plugin)
            end
            
            %
            % Report plugin
            %
            if obj.ReportEnable
                pdfFile = fullfile(obj.ResultFolder, obj.ReportFilename);
                if exist(pdfFile,"file") == 2
                    delete(pdfFile)
                end
                plugin = matlab.unittest.plugins.TestReportPlugin.producingPDF(pdfFile);
                runner.addPlugin(plugin)
            end
            
            %
            % Coverage plugin
            %
            if obj.CoverageEnable
                covFile = fullfile(obj.ResultFolder,obj.CoverageFilename);
                if exist(covFile,"file") == 2
                    delete(covFile)
                end
                reportFormat = matlab.unittest.plugins.codecoverage.CoberturaFormat(covFile);
                plugin = matlab.unittest.plugins.CodeCoveragePlugin.forPackage("ed247","Producing",reportFormat);
                runner.addPlugin(plugin)
            end
            
        end
        
        function testsuite = get.TestSuite(obj)
            testsuite = matlab.unittest.TestSuite.fromProject(obj.project_);
        end
        
    end
    
    %% PUBLIC METHODS
    methods
        
        function varargout = run(obj)
            
            results = matlab.unittest.TestResult.empty;
            status = -1;
            
            try
                
                if ~isfolder(obj.ResultFolder) && any([obj.CoverageEnable, obj.JUnitEnable, obj.ReportEnable, obj.TAPEnable])
                    mkdir(obj.ResultFolder)
                end
                
                results = obj.Runner.run(obj.TestSuite);
                status  = nnz([results.Failed]); % 0 = OK, > 0 = N tests failed
                
            catch me
                disp(me)
            end
            
            if nargout
                varargout = {status,results};
            end
            
        end
        
    end
    
    %% STATIC METHODS
    methods (Static)
        
        function runOnGitLabCI(varargin)
            
            proj = ci.openProject();
            obj = ci.Test(proj, ...
                "CoverageEnable",       true,               ...
                "CoverageFilename",     "Coverage.xml",     ...
                "JUnitEnable",          true,               ...
                "JUnitFilename",        "TestResults.xml"   ...
                );
            
            status = run(obj);
            quit(status)
            
        end
        
        function varargout = runOnLocal(varargin)
            
            proj = ci.openProject();
            
            obj = ci.Test(proj, ...
                "CoverageEnable",   true,               ...
                "CoverageFilename", "Coverage.xml",     ...
                "ResultFolder",     fullfile(proj.RootFolder, "results"), ...
                "ReportEnable",     true,               ...
                "ReportFilename",   "TestReport.pdf"    ...
                );
            
            [~, results] = run(obj);
            table(results)
            disp(results)
            
            if nargout
                varargout = {obj};
            end
            
        end
        
    end
    
end