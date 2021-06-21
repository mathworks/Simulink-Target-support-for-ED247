classdef Receive < ed247.blocks.aBlock
    %
    % Copyright 2021 The MathWorks, Inc.
    %
    
    %% CONSTANT
    properties (Constant)
        TYPES_FOR_REFRESH = {'A429','A664','A825'};
    end
    
    %% DEPENDENT PROPERTIES
    properties (Dependent)
        DisplayText
        IsRefresh
        PortLabel
    end
    
    %% CONSTRUCTOR
    methods
        
        function obj = Receive(block,varargin)
            obj@ed247.blocks.aBlock(block,varargin{:})
            obj.assert(strcmp(get(obj.block_,'ReferenceBlock'),'lib_ed247/ED247_Receive'), ...
                'Block shall be a ED247 Receive block')
        end
        
    end
    
    %% ACCESSORS
    methods
        
        function displaytext = get.DisplayText(obj)
            
            if bdIsLibrary(bdroot(obj.block_)) || strcmp(get(obj.block_,'show_port_labels'),'off')
                displaytext = 'ED247 Receive';
            else
                displaytext = '';
            end
            
        end
        
        function isrefresh = get.IsRefresh(obj)
            configuration = obj.Configuration;
            if ~isempty(configuration)
                isrefresh = any(ismember({configuration.signal_type},obj.TYPES_FOR_REFRESH));
            else
                isrefresh = true;
            end
        end
        
        function portlabel = get.PortLabel(obj)
            
            ports = getPorts(obj);
            
            if ~isempty(ports) && strcmp(get(obj.block_,'show_port_labels'),'on')
                
                portlabel = ports;
                
            else
                outports = get(obj.block_,'PortHandles');
                outports = outports.Outport;
                nports = numel(outports);
                portlabel = table(repmat({'output'},nports,1), (1:nports)', repmat({''},nports,1), ...
                    'VariableNames', {'Type','Number','Label'});
            end
            
        end
        
    end
    
    %% PUBLIC METHODS
    methods
        
        function initialize(obj)
            
            m = Simulink.Mask.get(obj.block_);
            refreshgroup = m.getDialogControl('refresh_group');
            
            if obj.IsRefresh
                refreshgroup.Visible = 'on';
            else
                refreshgroup.Visible = 'off';
                set(obj.block_,'refresh_factor','0')
            end
            
            portlabels = obj.PortLabel;
            
            outputports     = get(obj.block_,'PortHandles');
            outputports     = outputports.Outport;
            for iout = 1:min([height(portlabels),numel(outputports)])
                set(outputports(iout), 'Name', portlabels.Label{iout})
            end
            
        end
        
        function enableRefresh(obj)
            
            m = Simulink.Mask.get(obj.block_);
            refreshfactor = m.Parameters(strcmp({m.Parameters.Name},'refresh_factor'));
            
            userdata = get(obj.block_,'UserData');
            
            enablerefresh = get(obj.block_,'enable_refresh');
            if strcmp(enablerefresh,'on')
                refreshfactor.Enabled = 'on';
                if isfield(userdata,'RefreshFactor') && userdata.RefreshFactor > 0
                    set(obj.block_,'refresh_factor',num2str(userdata.RefreshFactor))
                else
                    set(obj.block_,'refresh_factor','1')
                end
            else
                refreshfactor.Enabled = 'off';
                userdata.RefreshFactor = str2double(get(obj.block_,'refresh_factor'));
                set(obj.block_,'UserData',userdata)
                set(obj.block_,'refresh_factor','0')
            end
            
        end
        
        function changeRefreshFactor(obj)
            refresh_factor = str2double(get(obj.block_,'refresh_factor'));
            if refresh_factor > 0
                set(obj.block_,'enable_refresh','on')
            else
                set(obj.block_,'enable_refresh','off')
            end
        end
        
        function createBus(obj)
            
            try
                
                hWait = waitbar(0, sprintf('Create bus for block %s', get(obj.block_,'Name')));
                hAx = findobj(hWait,'Type','Axes');
                hAx.Title.Interpreter = 'none';
                pause(0.1)
                
                parent = get(obj.block_,'Parent');
                blockname = get(obj.block_,'Name');
                position = get(obj.block_,'Position');
                
                refresh_factor = str2double(get(obj.block_,'refresh_factor'));
                isrefresh = refresh_factor > 0;
                
                configuration = obj.Configuration;
                outputsignals = configuration(ismember({configuration.direction},{'IN','INOUT'}));
                ports = getPorts(obj);
                
                waitbar(0.2, hWait, sprintf('Create main bus to regroup all received signals'))
                
                mainbusname = 'ReceiveOutputs';
                mainbus = add_block('simulink/Signal Routing/Bus Creator', sprintf('%s/%s', parent, mainbusname));
                set(mainbus, ...
                    'Position',     [position(3)+100,position(2),position(3)+110,position(4)], ...
                    'Inputs',       num2str(height(ports)), ...
                    'ShowName',     'off')
                
                elementnames = {outputsignals.name};
                                                
                if any(isrefresh)
                    
                    refreshmask = ismember({outputsignals.signal_type},obj.TYPES_FOR_REFRESH);
                    
                    waitbar(0.4, hWait, sprintf('Create intermediate bus for signal with refresh (%d/%d)', nnz(refreshmask), numel(refreshmask)))
                    
                    sigbusnames = strcat(elementnames(refreshmask),'_receive');
                    sigbus = cellfun(@(x) add_block('simulink/Signal Routing/Bus Creator', sprintf('%s/%s', parent, x),'MakeNameUnique','on'),sigbusnames);
                    
                    h = (position(4)-position(2))/numel(elementnames);
                    y1 = position(2)+h * (1:numel(elementnames));
                    y2 = y1 + h;
                    
                    arrayfun(@(b,x1,x2) set(b, ...
                        'Position',     [position(3)+45,x1,position(3)+55,x2], ...
                        'Inputs',       '2', ...
                        'ShowName',     'off'), sigbus, y1(refreshmask), y2(refreshmask))
                    
                    portrefreshmask = ismember(ports.Label,elementnames(refreshmask));
                    
                    waitbar(0.5, hWait, sprintf('Connect signals to intermediate buses'))
                    
                    l = arrayfun(@(i,b) add_line(parent, ...
                        sprintf('%s/%d', blockname, i), sprintf('%s/1',get(b,'Name'))), ...
                        ports.Number(portrefreshmask), sigbus(:));
                    set(l,'Name','data')
                    l = arrayfun(@(i,b) add_line(parent, ...
                        sprintf('%s/%d', blockname, i), sprintf('%s/2',get(b,'Name'))), ...
                        ports.Number(portrefreshmask)+1, sigbus(:));
                    set(l,'Name','refresh')
                    
                    source = arrayfun(@(x) sprintf('%s/%d',blockname,x), 1:height(ports), 'UniformOutput', false);
                    source(find(refreshmask)+1) = [];
                    source(refreshmask) = cellfun(@(x) sprintf('%s/1', x), sigbusnames, 'UniformOutput', false);
                    
                    allblocks = [obj.block_,mainbus,sigbus];
                    
                else
                    source = arrayfun(@(x) sprintf('%s/%d',blockname,x), 1:numel(elementnames), 'UniformOutput', false);
                    allblocks = [obj.block_,mainbus];
                end
                
                set(mainbus, 'Inputs', num2str(numel(elementnames)))
                
                waitbar(0.7, hWait, sprintf('Connect all signals to main bus'))
                
                l = arrayfun(@(s,i) add_line(parent, char(s), sprintf('%s/%d',mainbusname,i)), source, 1:numel(elementnames));
                arrayfun(@(x,y) set(x,'Name',char(y)), l, {outputsignals.name})
                
                waitbar(0.9, hWait, sprintf('Create subsystem for received signals'))
                
                tmpname = 'tmp_ed247_receive_with_bus';
                Simulink.BlockDiagram.createSubsystem(allblocks, 'Name', tmpname)
                set_param([parent,'/',tmpname],'Name',blockname)
                
                waitbar(1, hWait, sprintf('Bus and subsystem created successfully for block "%s"', blockname))
                pause(0.5)
                close(hWait)
                
            catch me
                
                delete(hWait)
                obj.error("Error while creating bus (%s):\n%s", me.identifier, me.message)
                                
            end
            
        end
        
    end
    
    %% PROTECTED METHODS
    methods (Access = protected)
        
        function ports = getPorts(obj)
            
            configuration   = obj.Configuration;
            refresh_factor = str2double(get(obj.block_,'refresh_factor'));
            isrefresh = refresh_factor > 0;
            
            if ~isempty(configuration)
                
                outputsignals = configuration(ismember({configuration.direction},{'IN','INOUT'}));
                
                names = {outputsignals.name};
                
                if isrefresh
                    names(2,:) = strcat({outputsignals.name},'_refresh');
                    names(2,~ismember({outputsignals.signal_type},obj.TYPES_FOR_REFRESH)) = {''};
                end
                
                names = names(~cellfun(@isempty,names));
                names = names(:);
                
                ports = table(repmat({'output'},size(names)), (1:length(names))', names, ...
                    'VariableNames', {'Type','Number','Label'});
                
            else
                ports = table();
            end
            
        end
        
    end
    
    %% STATIC METHODS
    methods (Static)
        
        function obj = fromCurrent(varargin)
            obj = ed247.blocks.Receive(gcbh);
        end
        
    end
    
end