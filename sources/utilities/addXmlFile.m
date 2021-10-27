function addXmlFile(modelName, dependencyObject)
%ADDXMLFILE Pack ECIC XML file in the MLDATX application
    eval([modelName,'([],[],[],''sizes'')']);
    findOpts = Simulink.FindOptions('IncludeCommented',false,...
        'MatchFilter', @Simulink.match.codeCompileVariants);
    configBlocks = Simulink.findBlocks(modelName, 'MaskType', 'Configurable block example', findOpts);
    xmlFiles = [];
    for block = configBlocks
        xmlFile = strrep(get_param(block,'configurationFilename'),'''','');
        xmlFiles(end+1).name = xmlFile;
        xmlFiles(end).path = which(xmlFile);
    end
    
    mldatxPath = fullfile(dependencyObject.Settings.LocalAnchorDir,[modelName,'.mldatx']);
    if isfile(mldatxPath)
        appObj = slrealtime.internal.Application(mldatxPath);
        for i = 1:length(xmlFiles)
            appObj.add(['/misc/' xmlFiles(i).name], xmlFiles(i).path);
        end
    end
end

