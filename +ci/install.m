function varargout = install(varargin)

status = -1;

try
    
    proj = slproject.getCurrentProjects();
    if isempty(proj)
        rootfolder = regexprep(mfilename('fullpath'),'\+.*','');
        proj = simulinkproject(rootfolder);
    end
    
    libxml2folder = getenv(ed247.Configuration.LIBXML2_FOLDER_VARIABLE);
    if ~isempty(libxml2folder)
        
        if ~isdir(libxml2folder) %#ok<ISDIR> Backward compatibility
            fprintf(1, '## Create folder "%s"\n', libxml2folder);
            mkdir(libxml2folder)
        end
        
        lixml2archive = fullfile(proj.RootFolder,'tests','_files','libxml2.zip');
        fprintf(1, '## Extract LibXML2 in folder "%s"\n', lixml2archive);
        unzip(lixml2archive,libxml2folder)
        
    end
    
    ed247folder = getenv(ed247.Configuration.ED247_FOLDER_VARIABLE);
    if ~isempty(ed247folder)
        
        if ~isdir(ed247folder) %#ok<ISDIR> Backward compatibility
            fprintf(1, '## Create folder "%s"\n', ed247folder);
            mkdir(ed247folder)
        end
        
        ed247archive = fullfile(proj.RootFolder,'tests','_files','ed247.zip');
        fprintf(1, '## Extract ED247 in folder "%s"\n', ed247folder);
        unzip(ed247archive,ed247folder)
        
    end
    
    toolboxfile = fullfile(proj.RootFolder, sprintf('ED247_for_Simulink-r%s.mltbx', version('-release')));
    if exist(toolboxfile,'file') == 2
        fprintf(1, '## Install toolbox ("%s")\n', toolboxfile);
        matlab.addons.toolbox.installToolbox(toolboxfile);
        status = 0;
    else
        status = 1;
    end
    
catch me
    disp(me.getReport())
end

if nargout
    varargout = {status};
end