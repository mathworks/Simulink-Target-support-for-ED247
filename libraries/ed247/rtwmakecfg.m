function makeInfo = rtwmakecfg
%
%
% Copyright 2021, The MathWorks Inc.
%

% ED247 configuration
config = ed247.Configuration.default();

makeInfo.includePath = { ...
	fullfile(config.Adapter,    'include'), ...
	fullfile(config.ED247,      'inc'), ...
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

		if strcmp(version('-release'),'2020b')
			qnxfolder = 'qnx700';
		elseif strcmp(version('-release'),'2021a')
			qnxfolder = 'qnx710';
		else
			error('Unsupported MATLAB release for use with Simulink Real-Time')
		end
		makeInfo.linkLibsObjs{end+1} = fullfile(getenv('SLREALTIME_QNX_SP_ROOT'),qnxfolder,'target','qnx7','x86_64','usr','lib','libxml2.so');
		makeInfo.linkLibsObjs{end+1} = fullfile(config.ED247,'lib','libed247_static.a');

        makeInfo.includePath{end+1}  = fullfile(getenv('SLREALTIME_QNX_SP_ROOT'),qnxfolder,'target','qnx7','x86_64','usr', 'include');
        
	otherwise

		makeInfo.linkLibsObjs{end+1} = fullfile(config.ED247,'lib','libed247.dll.a');
		makeInfo.linkLibsObjs{end+1} = fullfile(config.LibXML2,'lib','libxml2.a');
		makeInfo.linkLibsObjs{end+1} = fullfile(config.MinGW,'x86_64-w64-mingw32','lib','libws2_32.a');
        
        makeInfo.includePath{end+1}  = fullfile(config.LibXML2, 'include');

end

end