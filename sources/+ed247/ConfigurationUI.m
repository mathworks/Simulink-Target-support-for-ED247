classdef ConfigurationUI < matlab.mixin.SetGet
    %
    % Copyright 2020 The MathWorks, Inc.
    %
    
    %% HIDDEN PROPERTIES
    properties (Hidden)
        FgConfiguration
        PnConfiguration
        StCompilerFolder
        EdCompilerFolder
        PbBrowse
        PbOK
        PbCancel
    end
    
    %% PRIVATE PROPERTIES
    properties (Access = private)
        metadata_ = struct('Name',[],'Date',[],'Version',[],'Developer',[],'Maintainer',[]);
    end
    
    %% IMMUTABLE PROPERTIES
    properties (SetAccess = immutable, GetAccess = private)
        config_
        view_
    end
    
    %% CONSTRUCTOR
    methods
        
        function obj = ConfigurationUI(config,view,varargin)
            
            % Model
            obj.config_ = config;
            obj.view_ = view;
            
            if ~isempty(varargin)
                set(obj,varargin{:})
            end
                        
            % View
            update(obj);
            
            % Controller
            attachCallbacks(obj);
            
        end
        
    end
        
    %% CALLBACKS
    methods (Access = protected)
        
        function attachCallbacks(obj)
            
            % Menus
            obj.view_.CmSave.MenuSelectedFcn            = @(~,~) CmSaveMenuSelectedFcn(obj);
            obj.view_.CmClose.MenuSelectedFcn           = @(~,~) CmCloseMenuSelectedFcn(obj);
            obj.view_.CmReload.MenuSelectedFcn          = @(~,~) CmReloadMenuSelectedFcn(obj);
            obj.view_.CmGettingStarted.MenuSelectedFcn  = @(~,~) CmGettingStartedMenuSelectedFcn(obj);
            obj.view_.CmAbout.MenuSelectedFcn           = @(~,~) CmAboutMenuSelectedFcn(obj);
            
            % Buttons
            obj.view_.PbCompiler.ButtonPushedFcn    = @(~,~) PbCompilerCallback(obj);
            obj.view_.PbED247.ButtonPushedFcn       = @(~,~) PbED247Callback(obj);
            obj.view_.PbLibXML2.ButtonPushedFcn     = @(~,~) PbLibXML2Callback(obj);
            
            obj.view_.PbOK.ButtonPushedFcn          = @(~,~) PbOKCallback(obj);
            obj.view_.PbCancel.ButtonPushedFcn      = @(~,~) PbCancelCallback(obj);
            
            % Edit
            obj.view_.EdCompiler.ValueChangedFcn    = @(~,~) EdCompilerValueChangedFcn(obj);
            obj.view_.EdED247.ValueChangedFcn       = @(~,~) EdED247ValueChangedFcn(obj);
            obj.view_.EdLibXML2.ValueChangedFcn     = @(~,~) EdLibXML2ValueChangedFcn(obj);
            
            % Window
            obj.view_.FgConfiguration.CloseRequestFcn = @(~,~) FgConfigurationCloseRequestFcn(obj);
            
        end
        
        function PbCompilerCallback(obj)
            
            mingw = uigetdir(obj.config_.MinGW,'Select MinGW installation folder');
            obj.view_.FgConfiguration.Visible = 'off';
            obj.view_.FgConfiguration.Visible = 'on';
            if mingw == 0
                return
            end
            
            obj.config_.MinGW = mingw;
            update(obj)
            
        end
        
        function EdCompilerValueChangedFcn(obj)
            obj.config_.MinGW = obj.view_.EdCompiler.Value;
            update(obj)
        end
        
        function PbED247Callback(obj)
            
            ed247 = uigetdir(obj.config_.ED247,'Select ED247 library folder');
            obj.view_.FgConfiguration.Visible = 'off';
            obj.view_.FgConfiguration.Visible = 'on';
            if ed247 == 0
                return
            end
            
            obj.config_.ED247 = ed247;
            update(obj)
            
        end
        
        function EdED247ValueChangedFcn(obj)
            obj.config_.ED247 = obj.view_.EdED247.Value;
            update(obj)
        end
        
        function PbLibXML2Callback(obj)
            
            libxml2 = uigetdir(obj.config_.LibXML2,'Select ED247 library folder');
            obj.view_.FgConfiguration.Visible = 'off';
            obj.view_.FgConfiguration.Visible = 'on';
            if libxml2 == 0
                return
            end
            
            obj.config_.LibXML2 = libxml2;
            update(obj)
            
        end
        
        function EdLibXML2ValueChangedFcn(obj)
            obj.config_.LibXML2 = obj.view_.EdLibXML2.Value;
            update(obj)
        end
        
        function PbOKCallback(obj)
            save(obj.config_)
            close(obj.view_.FgConfiguration)
            projectOpen()
        end
        
        function PbCancelCallback(obj)
            close(obj.view_.FgConfiguration)
        end
        
        function FgConfigurationCloseRequestFcn(obj)
            delete(obj.view_.FgConfiguration)
        end
        
        function CmSaveMenuSelectedFcn(obj)
            save(obj.config_)
        end
        
        function CmCloseMenuSelectedFcn(obj)
            close(obj.view_.FgConfiguration)
        end
        
        function CmReloadMenuSelectedFcn(obj) %#ok<MANU>
            projectOpen()
        end
        
        function CmGettingStartedMenuSelectedFcn(obj) %#ok<MANU>
            gettingstarted = which('getting_started.html');
            web(gettingstarted)
        end
        
        function CmAboutMenuSelectedFcn(obj)
            
            readConfiguration(obj);
            
            f = uifigure('Name', sprintf('%s : About', obj.metadata_.Name), ...
                'Resize', 'off');
            f.Position(3:4) = [400,200];
            
            uilabel('Parent',f,'Text',obj.metadata_.Name, ...
                'Position',             [10 150 380 30], ...
                'FontSize',             20, ...
                'FontWeight',           'bold', ...
                'HorizontalAlignment',  'center');
                                    
            uilabel('Parent',f,'Text',sprintf('Version: %s', obj.metadata_.Version), ...
                'Position',             [10 120 380 15], ...
                'HorizontalAlignment',  'left');
            
            uilabel('Parent',f,'Text',sprintf('Date: %s', obj.metadata_.Date), ...
                'Position',             [10 95 380 15], ...
                'HorizontalAlignment',  'left');
            
            uilabel('Parent',f,'Text',sprintf('Developer: %s', obj.metadata_.Developer), ...
                'Position',             [10 70 380 15], ...
                'HorizontalAlignment',  'left');
            
            uilabel('Parent',f,'Text',sprintf('Maintainer: %s', obj.metadata_.Maintainer), ...
                'Position',             [10 45 380 15], ...
                'HorizontalAlignment',  'left');
            
        end
                
    end
    
    %% VIEW
    methods
        
        function update(obj)
            
            obj.view_.EdCompiler.Value = obj.config_.MinGW;
            obj.view_.EdED247.Value = obj.config_.ED247;
            obj.view_.EdLibXML2.Value = obj.config_.LibXML2;
            
        end
        
    end
    
    %% PROTECTED METHODS
    methods (Access = protected)
       
        function readConfiguration(obj)
           
            configurationfile = fullfile(ed247.Folder.ROOT.Path,'.metadata');
            fid = fopen(configurationfile);
            if fid > 0
                data = textscan(fid,'%s%s','Delimiter','=');
                fclose(fid);
            end
            
            data = [data{:}];
            obj.metadata_ = cell2struct(data(:,2),data(:,1),1);
            
        end
        
    end
    
    %% STATIC METHODS
    methods (Static)
        
        function varargout = open(varargin)
            
            config = ed247.Configuration.default();
            view = ed247.ConfigurationUIView();
            obj = ed247.ConfigurationUI(config, view, varargin{:});
            
            if nargout
                varargout = {obj};
            else
                setappdata(view.FgConfiguration, 'UIObject', obj);
            end
            
        end
        
    end
    
end