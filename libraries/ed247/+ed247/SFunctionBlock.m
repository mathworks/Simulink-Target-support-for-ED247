classdef SFunctionBlock < matlab.mixin.SetGet
    %
    % Copyright 2020 The MathWorks, Inc.
    %
    
    %% DEPENDENT PROPERTIES
    properties (Dependent)
        ConfigurationFileInt8
        DisplayText
        LogFileInt8
    end
    
    %% IMMUTABLE PROPERTIES
    properties (SetAccess = immutable, GetAccess = private)
        block_
    end
    
    %% CONSTRUCTOR
    methods
        
        function obj = SFunctionBlock(block,varargin)
            
            if isnumeric(block)
                block = strjoin({get(block,'Path'),get(block,'Name')},'/');
            end
            obj.block_ = block;
            
            if ~isempty(varargin)
                set(obj,varargin{:})
            end
            
        end
        
    end
    
    %% ACCESSORS
    methods
        
        function configurationfileint8 = get.ConfigurationFileInt8(obj)
            
            
            filename = get_param(obj.block_,'configurationFilename');
            if isempty(filename)
                configurationfileint8 = int8(0);
                return
            end
            
            configurationfile = regexp(filename,'^''(.*?)''$','tokens');            
            if isempty(configurationfile) && ~isempty(filename)                
                configurationfile = evalin('base',filename);                
            else
                configurationfile = char(configurationfile{1});
            end
            
            if ~isempty(which(configurationfile))
                configurationfileint8 = int8([which(configurationfile),0]);
            elseif exist(configurationfile,'file') == 2
                configurationfileint8 = int8([configurationfile,0]);
            else
                error('ED247:IOVariableNumber:invalidFile', ...
                    'Cannot find file %s', configurationfile)
            end
            
        end
        
        function displaytext = get.DisplayText(obj)
            
            filename = get_param(obj.block_,'configurationFilename');
            filename = regexprep(filename,'^''|''$','');
            
            if isempty(filename)
                displaytext = 'ED247';
            else
                [~,file,ext] = fileparts(filename);
                displaytext = [file,ext];
            end
            
        end
        
        function logfileint8 = get.LogFileInt8(obj)
            
            isenable = get_param(obj.block_,'enableLog');
            filename = get_param(obj.block_,'logFilename');
            if isempty(filename) || strcmp(isenable,'off')
                logfileint8 = int8(0);
                return
            end
            
           logfileint8 = regexp(filename,'^''(.*?)''$','tokens'); 
           logfileint8 = char(logfileint8{1});
           
           proj = slproject.getCurrentProjects();
            if numel(proj) == 1 && startsWith(logfileint8,'<PROJECT_FOLDER>') % A Simulink project is opened and selected path is relative to project folder
                logfileint8 = strrep(logfileint8,'<PROJECT_FOLDER>',proj.RootFolder);
            elseif startsWith(logfileint8,'<PROJECT_FOLDER>') % No project open -> use current folder
                logfileint8 = strrep(logfileint8,'<CURRENT_FOLDER>',pwd);
            elseif startsWith(logfileint8,'<CURRENT_FOLDER>')
                logfileint8 = strrep(logfileint8,'<CURRENT_FOLDER>',pwd);
            end
           
           logfileint8 = int8([logfileint8,0]); 
           
        end
        
    end
    
    %% PUBLIC METHODS
    methods
       
        function update(obj)
            
            enableLog = get_param(obj.block_,'enableLog');
            
            m = Simulink.Mask.get(obj.block_);
            if ~isempty(m.Parameters)
                logFilename = m.Parameters(strcmp({m.Parameters.Name},'logFilename'));
                browseLogFile   = m.getDialogControl('browseLogFile');
                                
                logFilename.Enabled = enableLog;
                browseLogFile.setEnabled(enableLog)
            end
            
        end
        
    end
    
    %% BLOCK CALLBACKS
    methods
       
        function InitFcn(obj)
            
            config = ed247.Configuration.default();
            config.addLibrariesToPath()
                   
            update(obj)
            
        end
        
    end
    
    %% PUBLIC METHODS
    methods
        
        function browseConfigurationFile(obj)
            
            filename = get_param(obj.block_,'configurationFilename');
            if ~isempty(strfind(filename,''''))
                filename = strrep(filename,'''','');
            elseif ~isempty(strfind(filename,'"'))
                filename = strrep(filename,'"','');
            end
            [filename,pathname] = uigetfile({'*.xml','ECIC configuration file'}, 'Select configuration file', filename);
            if pathname == 0; return; end
            
            mlpath = which(filename);
            if isempty(mlpath)
                filename = fullfile(pathname,filename);
            end
            set_param(obj.block_, 'configurationFilename', sprintf('''%s''', filename))
            
        end
        
        function browseLogFile(obj)
            
            filename = get_param(obj.block_,'logFilename');
            if ~isempty(strfind(filename,''''))
                filename = strrep(filename,'''','');
            elseif ~isempty(strfind(filename,'"'))
                filename = strrep(filename,'"','');
            end
            [filename,pathname] = uiputfile({'*.log','LOG file';'*.txt','TXT file';'*.*','All files'}, 'Define log file', filename);
            if pathname == 0; return; end
                      
            proj = slproject.getCurrentProjects();
            if numel(proj) == 1 && startsWith(pathname,proj.RootFolder) % A Simulink project is opened and selected path is relative to project folder
                pathname = strrep(pathname,proj.RootFolder,'<PROJECT_FOLDER>');
            elseif startsWith(pathname,pwd)
                pathname = strrep(pathname,pwd,'<CURRENT_FOLDER>');
            end
            
            filename = fullfile(pathname,filename);
            set_param(obj.block_, 'logFilename', sprintf('''%s''', filename))
            
        end
        
        function enableLogFile(obj)
            update(obj)
        end
        
    end
    
end