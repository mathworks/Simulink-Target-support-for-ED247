function varargout = test(varargin)

status  = -1;
results = matlab.unittest.TestResult.empty;

try
       
    rootfolder = regexprep(mfilename('fullpath'),'\+.*','');
            
    p = inputParser();
    p.addParameter('Mode',      'prod',   @(x) ismember(x,{'ci','dev','prod'}))
    p.addParameter('Coverage',  fullfile(rootfolder,sprintf('CoverageResults-r%s.xml',version('-release'))),@(x) validateattributes(x,{'string','char'},{'nonempty'}))
    p.addParameter('Report',    fullfile(rootfolder,sprintf('TestReport-r%s.pdf',version('-release'))),     @(x) validateattributes(x,{'string','char'},{'nonempty'}))
    p.addParameter('TAPFile',   fullfile(rootfolder,sprintf('TAPResults-r%s.tap',version('-release'))),     @(x) validateattributes(x,{'string','char'},{'nonempty'}))
    parse(p,varargin{:})
    
    proj = slproject.getCurrentProjects();
    if ismember(p.Results.Mode,{'ci','dev'}) && isempty(proj)
        fprintf(1, '## Open project ("%s")\n', rootfolder);
        simulinkproject(rootfolder);
    elseif ismember(p.Results.Mode,{'prod'})
        addpath(fullfile(rootfolder,'tests'))
    end
    
    fprintf(1, '## Create test suite\n');
    ts = matlab.unittest.TestSuite.fromPackage('ed247', 'IncludeSubpackages', true);
    
    fprintf(1, '## Create test runner\n');
    tr = matlab.unittest.TestRunner.withTextOutput();
    
    %
    % Coverage
    %
    if ismember(p.Results.Mode,{'ci','prod'}) && ~verLessThan('MATLAB','9.3') % r2017b
        reportFile = p.Results.Coverage;
        reportFormat = matlab.unittest.plugins.codecoverage.CoberturaFormat(reportFile);
        plugin = matlab.unittest.plugins.CodeCoveragePlugin.forPackage('ed247','Producing',reportFormat);
        fprintf(1, '## Enable Coverage plugin\n');
        tr.addPlugin(plugin)
    end
    
    %
    % TAP
    %
    if ismember(p.Results.Mode,{'ci','prod'})
        tapFile = p.Results.TAPFile;
        if exist(tapFile,'file') == 2
            fprintf(1, '## Delete previous TAP report ("%s")\n', tapFile);
            delete(tapFile)
        end
        tapFile = matlab.unittest.plugins.ToFile(tapFile);
        plugin  = matlab.unittest.plugins.TAPPlugin.producingVersion13(tapFile);
        fprintf(1, '## Enable TAP plugin\n');
        tr.addPlugin(plugin)
    end
    
    %
    % Report
    %
    if ismember(p.Results.Mode,{'ci','prod'})
        pdfFile = p.Results.Report;
        plugin = matlab.unittest.plugins.TestReportPlugin.producingPDF(pdfFile);
        fprintf(1, '## Enable Report plugin\n');
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