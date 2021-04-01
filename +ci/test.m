function varargout = test(varargin)

status  = -1;
results = matlab.unittest.TestResult.empty;

try
            
    proj = slproject.getCurrentProjects();
    if isempty(proj)
        rootfolder = regexprep(mfilename('fullpath'),'\+.*','');
        proj = simulinkproject(rootfolder);
    end
    
    p = inputParser();
    p.addParameter('Coverage',  fullfile(proj.RootFolder,sprintf('CoverageResults-r%s.xml',version('-release'))),@(x) validateattributes(x,{'string','char'},{'nonempty'}))
    p.addParameter('Report',    fullfile(proj.RootFolder,sprintf('TestReport-r%s.pdf',version('-release'))),     @(x) validateattributes(x,{'string','char'},{'nonempty'}))
    p.addParameter('TAPFile',   fullfile(proj.RootFolder,sprintf('TAPResults-r%s.tap',version('-release'))),     @(x) validateattributes(x,{'string','char'},{'nonempty'}))
    parse(p,varargin{:})
    
    ts = matlab.unittest.TestSuite.fromPackage('ed247', 'IncludeSubpackages', true);
    
    tr = matlab.unittest.TestRunner.withTextOutput();
    
    %
    % Coverage
    %
    if ~verLessThan('MATLAB','9.3') % r2017b
        reportFile = p.Results.Coverage;
        reportFormat = matlab.unittest.plugins.codecoverage.CoberturaFormat(reportFile);
        plugin = matlab.unittest.plugins.CodeCoveragePlugin.forPackage('ed247','Producing',reportFormat);
        tr.addPlugin(plugin)
    end
    
    %
    % TAP
    %
    tapFile = p.Results.TAPFile;
    tapFile = matlab.unittest.plugins.ToFile(tapFile);
    plugin  = matlab.unittest.plugins.TAPPlugin.producingVersion13(tapFile);
    tr.addPlugin(plugin)
    
    %
    % Report
    %
    pdfFile = p.Results.Report;
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