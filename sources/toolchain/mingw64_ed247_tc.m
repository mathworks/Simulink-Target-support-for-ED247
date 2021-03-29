function tc = mingw64_ed247_tc
%mingw64_ed247_tc Creates an MinGW64 ToolchainInfo object with specified
%   MinGW64 version for ED247
%
% >> tc = mingw64_ed247_tc;
% >> save mingw64_ed247_tc.mat tc
%
%
% Copyright 2020 The MathWorks SAS.

tc = coder.make.ToolchainInfo('BuildArtifact', 'gmake makefile');
tc.Name             = 'MinGW64 (Posix) | gmake makefile (64-bit Windows)';
tc.Platform         = 'win64';
tc.SupportedVersion = '4.9.2';

tc.addAttribute('TransformPathsWithSpaces');
tc.addAttribute('RequiresBatchFile');

% ------------------------------
% Setup
% ------------------------------
tc.MATLABSetup = { ...
    'initEnvironmentVariables()', ...
    };

% ------------------------------
% Macros
% ------------------------------
tc.addMacro('MW_LIB_DIR',           ['$(MATLAB_ROOT)\lib\' tc.Platform]);
tc.addMacro('CFLAGS_ADDITIONAL',    '');
tc.addMacro('CPPFLAGS_ADDITIONAL',  '');
tc.addMacro('LIBS_TOOLCHAIN',       '$(conlibs)');
tc.addMacro('CVARSFLAG',            '');

tc.addIntrinsicMacros({'BIN_MINGW_POSIX','ldebug', 'conflags', 'cflags'});

% ------------------------------
% C Compiler
% ------------------------------
 
tool = tc.getBuildTool('C Compiler');

tool.setName(           'MinGW C Compiler');
tool.setCommand(        'gcc');
tool.setPath(           '$(BIN_MINGW_POSIX)');

tool.setDirective(      'IncludeSearchPath',    '-I');
tool.setDirective(      'PreprocessorDefine',   '-D');
tool.setDirective(      'OutputFlag',           '-o');
tool.setDirective(      'Debug',                '-g');

tool.setFileExtension(  'Source',               '.c');
tool.setFileExtension(  'Header',               '.h');
tool.setFileExtension(  'Object',               '.obj');

tool.setCommandPattern('|>TOOL<| |>TOOL_OPTIONS<| |>OUTPUT_FLAG<| |>OUTPUT<|');

% ------------------------------
% C++ Compiler
% ------------------------------

tool = tc.getBuildTool('C++ Compiler');

tool.setName(           'MinGW C++ Compiler');
tool.setCommand(        'g++');
tool.setPath(           '$(BIN_MINGW_POSIX)');

tool.setDirective(      'IncludeSearchPath',  	'-I');
tool.setDirective(      'PreprocessorDefine', 	'-D');
tool.setDirective(      'OutputFlag',           '-o');
tool.setDirective(      'Debug',                '-g');

tool.setFileExtension(  'Source',               '.cpp');
tool.setFileExtension(  'Header',               '.hpp');
tool.setFileExtension(  'Object',               '.obj');

tool.setCommandPattern('|>TOOL<| |>TOOL_OPTIONS<| |>OUTPUT_FLAG<| |>OUTPUT<|');

% ------------------------------
% Linker
% ------------------------------

tool = tc.getBuildTool('Linker');

tool.setName(           'MinGW C Linker');
tool.setCommand(        'gcc');
tool.setPath(           '$(BIN_MINGW_POSIX)');

tool.setDirective(      'Library',              '-l');
tool.setDirective(      'LibrarySearchPath',    '-L');
tool.setDirective(      'OutputFlag',           '-o');
tool.setDirective(      'Debug',                '-g');

tool.setFileExtension(  'Executable',           '.exe');
tool.setFileExtension(  'Shared Library',       '.dll');

tool.setCommandPattern('|>TOOL<| |>TOOL_OPTIONS<| |>OUTPUT_FLAG<| |>OUTPUT<|');

% ------------------------------
% C++ Linker
% ------------------------------

tool = tc.getBuildTool('C++ Linker');

tool.setName(           'MinGW C++ Linker');
tool.setCommand(        'g++');
tool.setPath(           '$(BIN_MINGW_POSIX)');

tool.setDirective(      'Library',              '-l');
tool.setDirective(      'LibrarySearchPath',    '-L');
tool.setDirective(      'OutputFlag',           '-o');
tool.setDirective(      'Debug',                '-g');

tool.setFileExtension(  'Executable',           '.exe');
tool.setFileExtension(  'Shared Library',       '.dll');

tool.setCommandPattern('|>TOOL<| |>TOOL_OPTIONS<| |>OUTPUT_FLAG<| |>OUTPUT<|');

% ------------------------------
% Archiver
% ------------------------------

tool = tc.getBuildTool('Archiver');

tool.setName(       'MinGW Archiver');
tool.setCommand(    'ar');
tool.setPath(       '$(BIN_MINGW_POSIX)');

tool.setDirective(  'OutputFlag','');

tool.setFileExtension('Static Library','.lib');

tool.setCommandPattern('|>TOOL<| |>TOOL_OPTIONS<| |>OUTPUT_FLAG<| |>OUTPUT<|');

% ------------------------------
% Builder
% ------------------------------

tc.setBuilderApplication(tc.Platform);

% --------------------------------------------
% BUILD CONFIGURATIONS
% --------------------------------------------

optimsOffOpts    = {'-O0'};
cCompilerOpts    = {'-c $(cflags) $(CVARSFLAG) $(CFLAGS_ADDITIONAL)'};
linkerOpts       = {'$(ldebug) $(conflags) $(LIBS_TOOLCHAIN)'};
linkerAdditionalOpts = {};
archiverOpts     = {'ruvs'};

% Set the toolchain flags for 'Faster Builds' build configuration

cfg = tc.getBuildConfiguration('Faster Builds');
cfg.setOption( 'C Compiler',                horzcat(cCompilerOpts,   optimsOffOpts));
cfg.setOption( 'Linker',                    horzcat(linkerOpts,linkerAdditionalOpts));
cfg.setOption( 'Archiver',                  archiverOpts);

cfg = tc.getBuildConfiguration('Faster Runs');
cfg.setOption( 'C Compiler',                horzcat(cCompilerOpts,   optimsOffOpts));
cfg.setOption( 'Linker',                    horzcat(linkerOpts,linkerAdditionalOpts));
cfg.setOption( 'Archiver',                  archiverOpts);

cfg = tc.getBuildConfiguration('Debug');
cfg.setOption( 'C Compiler',                horzcat(cCompilerOpts,   optimsOffOpts));
cfg.setOption( 'Linker',                    horzcat(linkerOpts,linkerAdditionalOpts));
cfg.setOption( 'Archiver',                  archiverOpts);

tc.setBuildConfigurationOption('all', 'Download',      '');
tc.setBuildConfigurationOption('all', 'Execute',       '');
tc.setBuildConfigurationOption('all', 'Make Tool',     '-f $(MAKEFILE)');
