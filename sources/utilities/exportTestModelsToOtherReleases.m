function exportTestModelsToOtherReleases(basename)

currentfolder = pwd;
popd = onCleanup(@() cd(currentfolder));

proj = currentProject();
folder = fullfile(proj.RootFolder, "tests", "_files");
cd(folder)

current = "r" + version('-release');
versions = ["r2016b","r2017b","r2018b","r2019b","r2020b"];

modelname = basename + "_" + current;
if ~bdIsLoaded(modelname)
    load_system(modelname)
    closeModel = onCleanup(@() bdclose(modelname));
end

currentidx = find(versions == current);
for v = versions(1:(currentidx-1))
    Simulink.exportToVersion(modelname ,basename + "_" + v, v);
end

for v = versions((currentidx+1):end)
    
    if ~bdIsLoaded(modelname)
        load_system(modelname)
    end
    
    versionname = basename + "_" + v;
    fprintf(1, "## Save '%s'\n", fullfile(folder,versionname));
    
    save_system(modelname,fullfile(folder,versionname))
    close_system(versionname)
    
end