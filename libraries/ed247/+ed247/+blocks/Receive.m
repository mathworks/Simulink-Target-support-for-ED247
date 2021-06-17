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
            
            configuration   = obj.Configuration;
            refresh_factor = str2double(get(obj.block_,'refresh_factor'));
            isrefresh = refresh_factor > 0;
                
            if ~isempty(configuration) && strcmp(get(obj.block_,'show_port_labels'),'on')
                
                outputsignals = configuration(ismember({configuration.direction},{'IN','INOUT'}));
                
                if isrefresh
                    nports = numel(outputsignals) * 2;
                else
                    nports = numel(outputsignals);
                end
                
                portlabel = table(repmat({'output'},nports,1), (1:nports)', repmat({''},nports,1), ...
                    'VariableNames', {'Type','Number','Label'});
                                                
                iport = 1;
                for isig = 1:numel(outputsignals)
                    
                    basename = outputsignals(isig).name;
                    
                    % Data port
                    portlabel.Label{iport} = basename;
                    iport = iport + 1;
                    
                    % Refresh port
                    if isrefresh
                        portlabel.Label{iport} = sprintf('%s_refresh',basename);
                        iport = iport + 1;
                    end
                    
                end
                
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
            
            parent = get(obj.block_,'Parent');
            blockname = get(obj.block_,'Name');
            position = get(obj.block_,'Position');
            
            configuration   = obj.Configuration;
            
            refresh_factor = str2double(get(obj.block_,'refresh_factor'));
            isrefresh = refresh_factor > 0;
            
            mainbusname = 'ReceiveOutputs';
            mainbus = add_block('simulink/Signal Routing/Bus Creator', sprintf('%s/%s', parent, mainbusname));
            set(mainbus, ...
                'Position',     [position(3)+100,position(2),position(3)+110,position(4)], ...
                'Inputs',       num2str(numel(configuration)), ...
                'ShowName',     'off')
            
            elementnames = {configuration.name};
            
            sigbusnames = strcat(elementnames,'_receive');
            if isrefresh
                sigbus = cellfun(@(x) add_block('simulink/Signal Routing/Bus Creator', sprintf('%s/%s', parent, x),'MakeNameUnique','on'),sigbusnames);
                allbus = [mainbus,sigbus];
            else
                allbus = mainbus;
            end
            
            height = (position(4)-position(2))/numel(sigbusnames);
            y = [position(2),position(2)+height];
            for i_sig = 1:numel(sigbusnames)
                
                if isrefresh
                    
                    set(sigbus(i_sig), ...
                        'Position',     [position(3)+45,y(1),position(3)+55,y(2)], ...
                        'Inputs',       '2', ...
                        'ShowName',     'off')
                    y = [y(2),y(2)+height];
                    
                    data = add_line(parent, ...
                        sprintf('%s/%d', blockname, 1+(i_sig-1)*2), sprintf('%s/1',sigbusnames{i_sig}));
                    set(data,'Name','data')
                    refresh = add_line(parent, ...
                        sprintf('%s/%d', blockname, i_sig*2), sprintf('%s/2',sigbusnames{i_sig}));
                    set(refresh,'Name','refresh')
                    
                    source = sprintf('%s/1',sigbusnames{i_sig});
                    
                else
                    source = sprintf('%s/%d', blockname, i_sig);
                end
                
                sig = add_line(parent, source, sprintf('%s/%d',mainbusname,i_sig));
                set(sig,'Name',configuration(i_sig).name)
                
            end
            
            subname = sprintf('%s_bus',blockname);
            Simulink.BlockDiagram.createSubsystem(allbus, 'Name', subname)
            position = get_param([parent,'/',subname], 'Position');
            set_param([parent,'/',subname], 'Position', [position(1)+50,position(2),position(1)+55,position(4)], 'ShowName','off')
            
        end
        
    end
    
    %% STATIC METHODS
    methods (Static)
        
        function obj = fromCurrent(varargin)
            obj = ed247.blocks.Receive(gcbh);
        end
        
    end
    
end