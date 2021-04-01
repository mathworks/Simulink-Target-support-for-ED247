function varargout = test(varargin)

status  = -1;
results = matlab.unittest.TestResult.empty;

try
       
    rootfolder = regexprep(mfilename('fullpath'),'\+.*','');
            
    p = inputParser();
    p.addParameter('Mode',      'ci',   @(x) ismember(x,{'ci','dev','prod'}))
    p.addParameter('Coverage',  fullfile(rootfolder,sprintf('CoverageResults-r%s.xml',version('-release'))),@(x) validateattributes(x,{'string','char'},{'nonempty'}))
    p.addParameter('Report',    fullfile(rootfolder,sprintf('TestReport-r%s.pdf',version('-release'))),     @(x) validateattributes(x,{'string','char'},{'nonempty'}))
    p.addParameter('TAPFile',   fullfile(rootfolder,sprintf('TAPResults-r%s.tap',version('-release'))),     @(x) validateattributes(x,{'string','char'},{'nonempty'}))
    parse(p,varargin{:})
    
    proj = slproject.getCurrentProjects();
    if ismember(p.Results.Mode,{'ci','dev'}) && isempty(proj)
        simulinkproject(rootfolder);
    end
    
    ts = matlab.unittest.TestSuite.fromPackage('ed247', 'IncludeSubpackages', true);
    
    tr = matlab.unittest.TestRunner.withTextOutput();
    
    %
    % Coverage
    %
    if ismember(p.Results.Mode,{'ci'}) && ~verLessThan('MATLAB','9.3') % r2017b
        reportFile = p.Results.Coverage;
        reportFormat = matlab.unittest.plugins.codecoverage.CoberturaFormat(reportFile);
        plugin = matlab.unittest.plugins.CodeCoveragePlugin.forPackage('ed247','Producing',reportFormat);
        tr.addPlugin(plugin)
    end
    
    %
    % TAP
    %
    if ismember(p.Results.Mode,{'ci'})
        tapFile = p.Results.TAPFile;
        if exist(tapFile,'file') == 2
            delete(tapFile)
        end
        tapFile = matlab.unittest.plugins.ToFile(tapFile);
        plugin  = matlab.unittest.plugins.TAPPlugin.producingVersion13(tapFile);
        tr.addPlugin(plugin)
    end
    
    %
    % Report
    %
    if ismember(p.Results.Mode,{'ci'})
        pdfFile = p.Results.Report;
        plugin = matlab.unittest.plugins.TestReportPlugin.producingPDF(pdfFile);
        tr.addPlugin(plugin)
    end
    
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