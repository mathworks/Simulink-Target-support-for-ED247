function sl_customization(cm)

cm.registerTargetInfo(@loc_createToolchain);

end

% -------------------------------------------------------------------------
% Create the ToolchainInfoRegistry entries
% -------------------------------------------------------------------------
function config = loc_createToolchain

config(1)                       = coder.make.ToolchainInfoRegistry;
config(1).Name                  = 'MinGW64 (ED247) | gmake makefile (64-bit Windows)';
config(1).FileName              = fullfile(fileparts(mfilename('fullpath')), 'mingw64_ed247_tc.mat');
config(1).TargetHWDeviceType    = {'*'};
config(1).Platform              =  {computer('arch')};

end
