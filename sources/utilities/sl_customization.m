function sl_customization(cm) 
%
% Copyright 2020 The MathWorks, Inc.
%

initialize()

% Put ED247 library at the top of LIbrary Browser
cm.LibraryBrowserCustomizer.applyNodePreference({'lib_ed247',-42}); 
  
function initialize

if isdir(ed247.Folder.WORK.Path) %#ok<ISDIR>
    Simulink.fileGenControl('set', ...
        'CacheFolder',      ed247.Folder.WORK.Path, ...
        'CodeGenFolder',    ed247.Folder.WORK.Path ...
        );
end

% Set environment variable to use the right MinGW64
% /!\ This function is not generic and have to be updated depending on
% local installation
config = ed247.Configuration.default();

status = config.addMinGWToPath();

fprintf(1, '\n\n\n');
if ~status
        
    fprintf(1, '/!\\ MinGW64 compiler configuration is not valid.\n');
    fprintf(1, '## To set up MinGW64 compiler <a href="matlab:ed247.ConfigurationUI.open()">open UI</a> or follow these steps (command-line):\n');
    fprintf(1, '\t>> c = ed247.Configuration.default(); (<a href="matlab:c=ed247.Configuration.default();">Execute</a>)\n');
    fprintf(1, '\t>> c.MinGW = <path_to_mingw_installation_folder>; (Do NOT include bin/ subfolder)\n');
    fprintf(1, '\t>> save(c); (<a href="matlab:save(c);">Execute</a>)\n');
    fprintf(1, '\t>> projectOpen(); (<a href="matlab:projectOpen();">Execute</a>)\n');
    
else
    
    fprintf(1, '## ED247 library successfully configured\n');
    fprintf(1, '\t - <a href="matlab:web(''%s/ed247_index.html'')">Documentation</a>\n', ed247.Folder.DOC.Path);
    fprintf(1, '\t - <a href="matlab:web(''%s/demos.html'')">Demos</a>\n', ed247.Folder.DOC.Path);
    
end

config.addLibrariesToPath()