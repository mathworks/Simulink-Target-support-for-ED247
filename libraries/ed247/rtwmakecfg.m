function makeInfo = rtwmakecfg
%
%
% Copyright 2021, The MathWorks Inc.
%

% ED247 configuration
config = ed247.Configuration.default();

makeInfo.includePath = { ...
	fullfile(config.Adapter,    'include'), ...
	fullfile(config.ED247,      'include'), ...
	};
makeInfo.sourcePath = { ...
	fullfile(config.Adapter,    'src'), ...
	};
makeInfo.sources = { ...
	'ed247_interface.cpp',    ...
	'ed247_cmd_xml.cpp',      ...
	'tools.cpp',              ...
	};
makeInfo.linkLibsObjs = {};

sysTarget = get_param(bdroot, 'RTWSystemTargetFile');

switch sysTarget

	case 'slrealtime.tlc'

        qnxVersion = getenv('SLREALTIME_QNX_VERSION');
        if isempty(qnxVersion)
            qnxVersion = 'qnx700';
        end
		makeInfo.linkLibsObjs{end+1} = fullfile(getenv('SLREALTIME_QNX_SP_ROOT'),qnxVersion,'target','qnx7','x86_64','usr','lib','libxml2.so');
		makeInfo.linkLibsObjs{end+1} = fullfile(config.QNXLib,'lib','libed247_static.a');

        makeInfo.includePath{end+1}  = fullfile(getenv('SLREALTIME_QNX_SP_ROOT'),qnxVersion,'target','qnx7','x86_64','usr', 'include');
        
	otherwise

		makeInfo.linkLibsObjs{end+1} = fullfile(config.ED247,'lib','ed247.lib');
        
        makeInfo.includePath{end+1}  = fullfile(config.LibXML2, 'include');

end

end