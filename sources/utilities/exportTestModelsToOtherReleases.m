function exportTestModelsToOtherReleases(basename)

currentfolder = pwd;
popd = onCleanup(@() cd(currentfolder));

proj = currentProject();
folder = fullfile(proj.RootFolder, "tests", "_files");
cd(folder)

current = "r" + version('-release');
versions = ["r2016b","r2017b","r2018b","r2019b"];

modelname = basename + "_" + current;
if ~bdIsLoaded(modelname)
    load_system(modelname)
    closeModel = onCleanup(@() bdclose(modelname));
end

for v = versions
    Simulink.exportToVersion(modelname ,basename + "_" + v, v); 
end