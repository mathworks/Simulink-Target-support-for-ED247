function varargout = package(varargin)

status = -1;

try
    
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
    reset(config)
    
    %
    % Package toolbox
    %
    toolboxproject = fullfile(proj.RootFolder,'ToolboxPackagingConfiguration.prj');
    toolboxfile = fullfile(proj.RootFolder, sprintf('ED247_for_Simulink-r%s.mltbx', version('-release')));
    matlab.addons.toolbox.packageToolbox(toolboxproject, toolboxfile)
        
    %
    % Validate toolbox
    %
    tlbx = matlab.addons.toolbox.installToolbox(toolboxfile);
    uninstallTlbx = onCleanup(@() matlab.addons.toolbox.uninstallToolbox(tlbx));
    
    testfolder = fullfile(proj.RootFolder,'tests');
    close(proj);
    addpath(testfolder)
    
    status = ci.test('Mode','prod');
    rmpath(testfolder)
    
catch me
   disp(me.getReport())    
end

if nargout
    varargout = {status};
end