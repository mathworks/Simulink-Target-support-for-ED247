classdef Send < ed247.blocks.aBlock
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
        
        function obj = Send(block,varargin)
            obj@ed247.blocks.aBlock(block,varargin{:})
        end
        
    end
    
    %% ACCESSORS
    methods
        
        function displaytext = get.DisplayText(obj)
            
            if bdIsLibrary(bdroot(obj.block_)) || strcmp(get(obj.block_,'show_port_labels'),'off')
                displaytext = 'ED247 Send';
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
            
            inputports     = get(obj.block_,'PortHandles');
            inputports     = inputports.Inport;
            
            portlabel = table(repmat({'input'},numel(inputports),1), (1:numel(inputports))', repmat({''},numel(inputports),1), ...
                'VariableNames', {'Type','Number','Label'});
            
            configuration   = obj.Configuration;
            
            if ~isempty(configuration) && strcmp(get(obj.block_,'show_port_labels'),'on')
                
                inputsignals   = configuration(ismember({configuration.direction},{'OUT','INOUT'}));
                
                isrefresh = obj.IsRefresh && strcmp(get(obj.block_,'enable_refresh'),'on');
                
                iport = 1;
                for isig = 1:numel(inputsignals)
                    
                    basename = inputsignals(isig).name;
                                  
                    % Data port
                    portlabel.Label{iport} = basename;
                    iport = iport + 1;
                    
                    % Refresh port
                    if isrefresh && height(portlabel) > numel(inputsignals)
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
            
            enablerefresh = m.Parameters(strcmp({m.Parameters.Name},'enable_refresh'));
            enablerefresh.Visible = 'off';
            enablerefresh.Enabled = 'off';
            
            set(obj.block_,'enable_refresh','off')
            
        end
                
    end
    
    %% STATIC METHODS
    methods (Static)
        
        function obj = fromCurrent(varargin)
            obj = ed247.blocks.Send(gcbh);
        end
        
    end
    
end