function makeInfo = rtwmakecfg

% proj = currentProject;
% rootPath = proj.RootFolder;

% ED247 configuration
config = ed247.Configuration.default();

makeInfo.linkLibsObjs = {};

sysTarget = get_param(bdroot, 'RTWSystemTargetFile');

switch sysTarget
    
    case 'slrealtime.tlc'
        
        makeInfo.includePath = { ...
            fullfile(config.ED247, 'inc'), ...
            fullfile(config.LibXML2, 'include'), ...
            };
        
        %{
        makeInfo.includePath = fullfile(rootPath,'_install_qnx/include');
        makeInfo.linkLibsObjs{end+1} = fullfile(rootPath,'_install_qnx/lib/libecal_core.so');
        makeInfo.linkLibsObjs{end+1} = fullfile(rootPath,'_install_qnx/lib/libecal_core_c.so');
        makeInfo.linkLibsObjs{end+1} = fullfile(rootPath,'_install_qnx/lib/libprotobuf.so');
        makeInfo.linkLibsObjs{end+1} = fullfile(rootPath,'_install_qnx/lib/libprotobuf-lite.so');
        %}
        
    otherwise
        
        makeInfo.includePath = { ...
            fullfile(config.Adapter,    'include'), ...
            fullfile(config.ED247,      'inc'), ...
            fullfile(config.LibXML2,    'include'), ...
            };
        makeInfo.sourcePath = { ...
            fullfile(config.Adapter,    'src'), ...
            };
        makeInfo.sources = { ...
            'ed247_interface.c',    ...
            'ed247_cmd_xml.c',      ...
            'tools.c',              ...
            };
        
        makeInfo.linkLibsObjs{end+1} = fullfile(config.ED247,'lib','libed247.dll.a');
        makeInfo.linkLibsObjs{end+1} = fullfile(config.LibXML2,'lib','libxml2.a');
        makeInfo.linkLibsObjs{end+1} = fullfile(config.MinGW,'x86_64-w64-mingw32','lib','libws2_32.a');
        
end

end