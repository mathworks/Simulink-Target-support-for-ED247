classdef Folder
    % FOLDER
    %
    % Copyright 2020 The MathWorks, Inc.
    %
    enumeration
        
        ADAPTER         ({'c_sources'})
        DEMOS           ({'demos'})
        DOC             ({'doc','html'})
        LIBRARY         ({'libraries','ed247'})
        ROOT            ({''})
        WORK            ({'work'})
        
    end
    
    %% DEPENDENT PROPERTIES
    properties (Dependent)
        Path
    end
    
    %% IMMUTABLE PROPERTIES
    properties (SetAccess = immutable, GetAccess = private)
        path_
    end
    
    %% CONSTRUCTOR
    methods
        
        function obj = Folder(subfolders)
            
            %rootdir = fileparts(fileparts(fileparts(fileparts(mfilename('fullpath')))));
            rootdir = regexprep(which('lib_ed247'),'libraries.*','');
            obj.path_ = fullfile(rootdir,subfolders{:});
            
        end
        
    end
    
    %% ACCESSORS
    methods
        
        function path = get.Path(obj)
            path = obj.path_;
        end
        
    end
    
    
end