function varargout = test(varargin)

status  = -1;
results = matlab.unittest.TestResult.empty;

try
    
    proj = slproject.getCurrentProjects();
    if isempty(proj)
        rootfolder = regexprep(mfilename('fullpath'),'\+.*','');
        proj = simulinkproject(rootfolder);
    end
    
    ts = matlab.unittest.TestSuite.fromPackage('ed247', 'IncludeSubpackages', true);
    
    tr = matlab.unittest.TestRunner.withTextOutput();
    
    %
    % Coverage
    %
    if ~verLessThan('MATLAB','9.3') % r2017b
        reportFile = fullfile(proj.RootFolder,'CoverageResults.xml');
        reportFormat = matlab.unittest.plugins.codecoverage.CoberturaFormat(reportFile);
        plugin = matlab.unittest.plugins.CodeCoveragePlugin.forPackage('ed247','Producing',reportFormat);
        tr.addPlugin(plugin)
    end
    
    %
    % TAP
    %
    tapFile = fullfile(proj.RootFolder,'TAPResults.tap');
    tapFile = matlab.unittest.plugins.ToFile(tapFile);
    plugin  = matlab.unittest.plugins.TAPPlugin.producingVersion13(tapFile);
    tr.addPlugin(plugin)
    
    %
    % Report
    %
    pdfFile = fullfile(proj.RootFolder,'TestReport.pdf');
    plugin = matlab.unittest.plugins.TestReportPlugin.producingPDF(pdfFile);
    tr.addPlugin(plugin)
    
    %
    % Run and get results
    %
    results = tr.run(ts);
    disp(results)
    status = nnz([results.Failed]);
    
catch me
    
    disp(me.getReport())
    
end

if nargout
    varargout = {status,results};
end