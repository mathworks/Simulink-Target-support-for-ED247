classdef Compile < matlab.mixin.SetGet
               
    %% IMMUTABLE PROPERTIES
    properties (SetAccess = immutable, GetAccess = private)
        rootfolder_             (1,1)   string
    end
    
    %% CONSTRUCTOR
    methods
       
        function obj = Compile(rootfolder, varargin)                        
            obj.rootfolder_         = rootfolder;
            if ~isempty(varargin)
                set(obj, varargin{:})
            end
        end
        
    end
    
    %% PUBLIC METHODS
    methods
        
        function createMEX(obj)
                        
            obj.print("Compile MEX files")
            ed247.compile()
            
        end
              
    end
    
    %% HELPER METHODS (PROTECTED)
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
            fprintf(1,varargin{1} + "\n",varargin{2:end})
        end
        
        function rethrow(obj,varargin) %#ok<INUSL>
            rethrow(varargin{:})
        end
        
    end
    
    %% STATIC METHODS
    methods (Static)
       
        function varargout = runOnLocal(varargin)
                   
            proj = ci.openProject();
            
            obj = ci.Compile( proj.RootFolder);
                        
            if nargout
                varargout = {obj};
            end
            
        end
        
        function runOnGitLabCI(varargin)
           
            status = -1;
            
            try
                
                proj = ci.openProject();
                                
                c = ci.Compile( proj.RootFolder);
                createMEX(c)
                
                status = 0;
                
            catch me
                disp(me.getReport())
            end
            
            quit(status)
            
        end
        
    end
    
end