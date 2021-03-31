function varargout = cleanup(varargin)

status = -1;

try
    
    proj = slproject.getCurrentProjects();
    if isempty(proj)
        rootfolder = regexprep(mfilename('fullpath'),'\+.*','');
        simulinkproject(rootfolder);
    end
   
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
    
    status = 0;
    
catch me
    disp(me.getReport())
end

if nargout
    varargout = {status};
end