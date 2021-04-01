function varargout = cleanup(varargin)

status = -1;

try
       
    libxml2folder = getenv(ed247.Configuration.LIBXML2_FOLDER_VARIABLE);
    if ~isempty(libxml2folder) && ~isdir(libxml2folder) %#ok<ISDIR> Backward compatibility
        fprintf(1, '## Delete folder "%s"\n', libxml2folder);
        rmdir(libxml2folder,'s')
    end
    
    ed247folder = getenv(ed247.Configuration.ED247_FOLDER_VARIABLE);
    if ~isempty(ed247folder) && ~isdir(ed247folder) %#ok<ISDIR> Backward compatibility
        fprintf(1, '## Delete folder "%s"\n', ed247folder);
        rmdir(ed247folder,'s')
    end
    
    toolboxes = matlab.addons.toolbox.installedToolboxes();
    if any(strcmp({toolboxes.Name},'ED247_for_Simulink'))
        tlbx = toolboxes(strcmp({toolboxes.Name},'ED247_for_Simulink'));
        fprintf(1, '## Uninstall toolbox ("%s")\n', tlbx.Name);
        matlab.addons.toolbox.uninstallToolbox(tlbx);
    end
    
    status = 0;
    
catch me
    disp(me.getReport())
end

if nargout
    varargout = {status};
end