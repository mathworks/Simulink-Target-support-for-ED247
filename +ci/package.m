function varargout = package(varargin)

status = -1;

try
    
    p = inputParser();
    p.addParameter('Validate',true,@(x) validateattributes(x,{'logical'},{'scalar'}))
    parse(p,varargin{:})
    
    proj = slproject.getCurrentProjects();
    if isempty(proj)
        rootfolder = regexprep(mfilename('fullpath'),'\+.*','');
        proj = simulinkproject(rootfolder);
    else
        reopenProject = onCleanup(@() simulinkproject(proj.RootFolder));
    end
        
    config = ed247.Configuration.default();    
    copyfile(config.Filename, [config.Filename,'.bckp'])
    resetMetadata = onCleanup(@() movefile([config.Filename,'.bckp'],config.Filename));
    
    fprintf(1, '## Reset .metadata file ("%s")\n', config.Filename);
    reset(config)
    
    %
    % Package toolbox
    %
    toolboxproject = fullfile(proj.RootFolder,'ToolboxPackagingConfiguration.prj');
    toolboxfile = fullfile(proj.RootFolder, sprintf('ED247_for_Simulink-r%s.mltbx', version('-release')));
    
    fprintf(1, '## Package toolbox into "%s"\n', toolboxfile);
    matlab.addons.toolbox.packageToolbox(toolboxproject, toolboxfile)
        
    %
    % Validate toolbox
    %
    if p.Results.Validate
        fprintf(1, '## Install toolbox ("%s")\n', toolboxfile);
        tlbx = matlab.addons.toolbox.installToolbox(toolboxfile);
        uninstallTlbx = onCleanup(@() matlab.addons.toolbox.uninstallToolbox(tlbx));
        
        fprintf(1, '## Prepare toolbox validation (close project and set MATAB path)\n');
        testfolder = fullfile(proj.RootFolder,'tests');
        close(proj);
        addpath(testfolder)
        
        fprintf(1, '## Run tests on installed toolbox\n');
        status = ci.test('Mode','prod');
        rmpath(testfolder)
    else
        status = 0;
    end
    
catch me
   disp(me.getReport())    
end

if nargout
    varargout = {status};
end