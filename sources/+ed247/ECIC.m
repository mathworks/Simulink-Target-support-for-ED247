classdef ECIC < matlab.mixin.SetGet
   
    %% IMMUTABLE PROPERTIES
    properties (SetAccess = immutable, GetAccess = private)
        filename_
    end
    
    %% CONSTRUCTOR
    methods
       
        function obj = ECIC(filename,varargin)
            obj.filename_ = filename;
            if ~isempty(varargin)
                set(obj,varargin{:})
            end
        end
        
    end
    
    %% PUBLIC METHODS
    methods
       
        function read(obj)
            
            obj.assert(exist(obj.filename_,'file') == 2, 'File "%s" does not exist', obj.filename_)
            
            txt = fileread(obj.filename_);
            streams = regexp(txt,'\<Streams?>(.*?)\</Streams?>','tokens');
            if ~isempty(streams)
                streams = char(streams{1});
            end
            
        end
        
    end
    
    %% PROTECTED HELPERS
    methods (Access = protected)
       
        function assert(obj,varargin) %#ok<INUSL>
            assert(varargin{:})
        end
        
        function error(obj,varargin) %#ok<INUSL>
            error(varargin{:})
        end
        
        function warning(obj,varargin) %#ok<INUSL>
            warning(varargin{:})
        end
        
        function print(obj,varargin) %#ok<INUSL>
            fprintf(1,varargin{:})
        end
        
    end
    
end