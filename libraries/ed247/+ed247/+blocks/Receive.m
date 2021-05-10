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
            
            outputports     = get(obj.block_,'PortHandles');
            outputports     = outputports.Outport;
            
            portlabel = table(repmat({'output'},numel(outputports),1), (1:numel(outputports))', repmat({''},numel(outputports),1), ...
                'VariableNames', {'Type','Number','Label'});
            
            configuration   = obj.Configuration;
            
            if ~isempty(configuration) && strcmp(get(obj.block_,'show_port_labels'),'on')
                
                outputsignals   = configuration(ismember({configuration.direction},{'IN','INOUT'}));
                
                refresh_factor = str2double(get(obj.block_,'refresh_factor'));
                isrefresh = refresh_factor > 0;
                
                iport = 1;
                for isig = 1:numel(outputsignals)
                    
                    basename = outputsignals(isig).name;
                    
                    % Data port                    
                    portlabel.Label{iport} = basename;
                    iport = iport + 1;
                    
                    % Refresh port
                    if isrefresh && height(portlabel) > numel(outputsignals)
                        portlabel.Label{iport} = sprintf('%s_refresh',basename);
                        iport = iport + 1;
                    end
                    
                end
                
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
            for iout = 1:numel(outputports)
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
                
    end
    
    %% STATIC METHODS
    methods (Static)
        
        function obj = fromCurrent(varargin)
            obj = ed247.blocks.Receive(gcbh);
        end
        
    end
    
end