function makeInfo = rtwmakecfg()
%RTWMAKECFG adds include and source directories to the make files.
%    makeInfo=rtwmakecfg returns a structured array containing build info.
%    Please refer to the rtwmakecfg API section in the Simulink Coder
%    documentation for details on the format of this structure.
%    Simulink version    : 8.8 (R2016b) 25-Aug-2016
%    MATLAB file generated on : 27-Mar-2020 13:19:29
%
% Copyright 2020 The MathWorks, Inc.
%

% Verify the Simulink version
%verify_simulink_version();

% Get the current directory
currDir = pwd;

% Get the MATLAB search path, remove the toolbox sub-directories except LCT demos
matlabPath = legacycode.lct.util.getSearchPath();

% Declare cell arrays for storing the paths
allIncPaths = {};
allSrcPaths = {};

% Get the build type
isSimTarget = is_simulation_target();
allLibs = {};
% Get the serialized paths information
info = get_serialized_info();

% Get all S-Function's name in the current model
sfunNames = {};
if ~isempty(bdroot)
    sfunBlks = find_system(bdroot,...
        'LookUnderMasks', 'all',...
        'FollowLinks', 'on',...
        'BlockType', 'S-Function'...
    );
    sfunNames = get_param(sfunBlks, 'FunctionName');
end

for ii = 1:numel(info)
    sfunInfo = info(ii);
    % If the S-Function isn't part of the current build then skip its path info
    if ~any(strcmp(sfunInfo.SFunctionName, sfunNames))
        continue
    end

    % Call the helper
    pathInfo = legacycode.lct.util.resolvePaths(sfunInfo, isSimTarget==1, currDir, matlabPath);

    % Concatenate known include and source directories
    allIncPaths = RTW.uniquePath([allIncPaths pathInfo.IncPaths(:)']);
    allSrcPaths = RTW.uniquePath([allSrcPaths pathInfo.SrcPaths(:)']);

    % Concatenate Host or Target libraries
    allLibs = RTW.uniquePath([allLibs pathInfo.LibFiles(:)']);
end

% Additional include directories
makeInfo.includePath = correct_path_name(allIncPaths);

% Additional source directories
makeInfo.sourcePath = correct_path_name(allSrcPaths);

% Additional libraries according to the build type
makeInfo.linkLibsObjs = correct_path_name(allLibs);

%% ------------------------------------------------------------------------
function info = get_serialized_info()

% Allocate the output structure array
info(1:1) = struct(...
    'SFunctionName', '',...
    'IncPaths', {{}},...
    'SrcPaths', {{}},...
    'LibPaths', {{}},...
    'SourceFiles', {{}},...
    'HostLibFiles', {{}},...
    'TargetLibFiles', {{}},...
    'singleCPPMexFile', false,...
    'Language', ''...
    );

config = ed247.Configuration.default();

info(1).SFunctionName   = 'ed247_sfun';
info(1).IncPaths        = { ...
    fullfile(config.Adapter,    'include'), ...
    fullfile(config.ED247,      'inc'), ...
    fullfile(config.LibXML2,    'include'), ...
    };
info(1).SrcPaths        = { ...
    fullfile(config.Adapter,    'src'), ...  
    };
info(1).SourceFiles     = { ...
    'ed247_interface.c',    ...
    'ed247_cmd_xml.c',      ...
    'tools.c',              ...
    };

if isunix()
    
    info(1).HostLibFiles = { ...
        'libed247.so', ...
        'libmemhooks.so', ... % Do not compile without this library (maybe issue with the way ED247 library is compiled)
        'libxml2.so', ...        
        };
    info(1).TargetLibFiles = info(1).HostLibFiles;
    info(1).LibPaths = { ...
        fullfile(config.ED247,'lib'), ... 
        fullfile('/usr', 'local','lib'), ...
        };
    
else
    
    info(1).HostLibFiles = { ...
        'libed247.dll.a', ...
        'libxml2.a', ...
        'libws2_32.a', ...
        };
    info(1).TargetLibFiles = info(1).HostLibFiles;
    info(1).LibPaths = { ...
        fullfile(config.ED247,'lib'), ... 
        fullfile(config.LibXML2, 'lib'), ...
        fullfile(config.MinGW,'x86_64-w64-mingw32','lib'), ...
        };
    
end

info(1).Language = 'C';

%% ------------------------------------------------------------------------
function verify_simulink_version()

% Retrieve Simulink version
slVer = ver('simulink');
factor = 1.0;
thisVer = 0.0;
for ii = 1:numel(slVer.Version)
    if slVer.Version(ii)=='.'
        factor = factor/10.0;
    else
        thisVer = thisVer + sscanf(slVer.Version(ii), '%d')*factor;
    end
end

% Verify that the actual platform supports the function used
if thisVer < 8.7
    DAStudio.error('Simulink:tools:LCTErrorBadSimulinkVersion', sprintf('%g',thisVer))
end

%% ------------------------------------------------------------------------
function pathSet = correct_path_name(pathSet)

for ii = 1:numel(pathSet)
    pathSet{ii} = rtw_alt_pathname(pathSet{ii});
end
pathSet = RTW.uniquePath(pathSet);

%% ------------------------------------------------------------------------
function isSimTarget = is_simulation_target()

% Default output value
isSimTarget = 0;

% Get the current model and the code generation type to decide
% if we must link with the host libraries or with the target libraries
try
    modelName = get_param(0, 'CurrentSystem');
    if ~isempty(modelName)
        modelName = bdroot(modelName);
        sysTarget = get_param(modelName, 'RTWSystemTargetFile');
        isSimTarget = ~isempty([strfind(sysTarget, 'rtwsfcn') strfind(sysTarget, 'accel')]);

        mdlRefSimTarget = get_param(modelName,'ModelReferenceTargetType');
        isSimTarget = strcmpi(mdlRefSimTarget, 'SIM') || isSimTarget;

        % Verify again it's not Accelerator
        if ~isSimTarget
            simMode = get_param(modelName, 'SimulationMode');
            simStat = get_param(modelName, 'SimulationStatus');
            isSimTarget = strcmp(simStat, 'initializing') & strcmp(simMode, 'accelerator');
        end
    end
catch
end
