function initEnvironmentVariables()

config = ed247.Configuration.default();
config.addLibrariesToPath()

setenv('BIN_MINGW_POSIX',fullfile(config.MinGW,'bin'))



