function sl_customization(cm) 
%
% Copyright 2020 The MathWorks, Inc.
%

initialize()

% Put ED247 library at the top of Library Browser
try %#ok<TRYNC>
    cm.LibraryBrowserCustomizer.applyNodePreference({'lib_ed247',-42});
end
  
cm.addCustomMenuFcn('Simulink:ContextMenu', @getMyMenuItems);

% Get default (factory) customizations
hObj = cm.RTWBuildCustomizer;

% Register build process hooks
hObj.addUserHook('exit', 'addXmlFile(modelName, dependencyObject);');

end

function schemaFcns = getMyMenuItems(~)

schemaFcns = {};

if strcmp(get(gcbh,'ReferenceBlock'),'lib_ed247/ED247_Receive')
    
    obj = ed247.blocks.Receive(gcbh);
    schemaFcns = { ...
        @(~) getCreateBus(obj); ...
        };
    
elseif ~isempty(getfield(get(gcbh,'PortHandles'),'Outport')) && ...
        numel(find_system(bdroot(gcbh), 'SearchDepth', 1, 'ReferenceBlock', 'lib_ed247/ED247_Send')) == 1
    
    block = find_system(bdroot(gcbh), 'SearchDepth', 1, 'ReferenceBlock', 'lib_ed247/ED247_Send');
    obj = ed247.blocks.Send(block);
    schemaFcns = { ...
        @(~) getConnectLine(obj, gcbh); ...
        };
    
end

end

function schema = getCreateBus(obj)

schema = sl_action_schema;
schema.label = '[ED247] Create Bus and subsystem';
schema.userdata = 'ed247_create_bus';
schema.callback = @(varargin) createBus(obj);

end

function schema = getConnectLine(obj,source)

schema = sl_action_schema;
schema.label = '[ED247] Connect line to Send block';
schema.userdata = 'ed247_connect_line';
schema.callback = @(varargin) connectLine(obj,source);

end
            
function initialize

% Set environment variable to use the right MinGW64
% /!\ This function is not generic and have to be updated depending on
% local installation
config = ed247.Configuration.default();

config.addLibrariesToPath()

end