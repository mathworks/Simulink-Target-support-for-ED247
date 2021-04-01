function varargout = package(varargin)

status = -1;

try
        
    proj = slproject.getCurrentProjects();
    if isempty(proj)
        rootfolder = regexprep(mfilename('fullpath'),'\+.*','');
        proj = simulinkproject(rootfolder);    
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
                
catch me
   disp(me.getReport())    
end

if nargout
    varargout = {status};
end