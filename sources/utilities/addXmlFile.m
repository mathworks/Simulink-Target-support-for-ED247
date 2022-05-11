function addXmlFile(modelName, dependencyObject)
%ADDXMLFILE Pack ECIC XML file in the MLDATX application
%
%
% Get list of Configuration blocks (from ED247 library) in the model
%
% Copyright 2021 The MathWorks, Inc.
%

% eval([char(modelName),'([],[],[],''sizes'')']); % cf. geck 2615990 : https://komodo.mathworks.com/main/gecko/view?Record=2615990&CURRENT_PAGE=dashboardclose

findOpts = Simulink.FindOptions('IncludeCommented',false,...
    'MatchFilter', @Simulink.match.codeCompileVariants);
configBlocks = Simulink.findBlocks(modelName, 'MaskType', 'Configurable block example', findOpts);

if ~isempty(configBlocks)
    
    % Get information about Configuration block
    %   Use ed247.blocks.Configure which is defined to manipulate block
    %   properties (no need to re-write GET_PARAM)
    %
    blockInfo = arrayfun(@ed247.blocks.Configure,configBlocks);
    
    %
    % Assemble the list of XML files :
    %   - ECIC : One per Configuration block
    %   - ICD : From 0 to many depending on ECIC configuration
    %
    xmlpaths = [{blockInfo.ECICFile}'; vertcat(blockInfo.ICDFiles)];
    xmlpaths(cellfun(@isempty,xmlpaths)) = []; % Empty XML path leads to ADD command crash
    
    %
    % Generate the structure xmlFiles which contains the name of each XML file
    % + the full path
    %
    [~,xmlnames] = fileparts(xmlpaths);
    xmlnames = cellfun(@(x) [x,'.xml'], cellstr(xmlnames), 'UniformOutput', false);
    xmlFiles = struct( ...
        "name",     xmlnames, ...
        "path",     xmlpaths);
    
    %
    % Copy the XML files into the MLDATX (generated for Simulink Real-Time)
    %
    mldatxPath = fullfile(dependencyObject.Settings.LocalAnchorDir,[modelName,'.mldatx']);
    if isfile(mldatxPath)
        appObj = slrealtime.internal.Application(mldatxPath);
        for i = 1:length(xmlFiles)
            appObj.add(['/misc/' xmlFiles(i).name], xmlFiles(i).path);
        end
    end
    
end

end

