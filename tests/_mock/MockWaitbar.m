classdef MockWaitbar < matlab.mixin.SetGet
   % MOCKWAITBAR Mock the WAITBAR function for testing purpose
   
   %% CONSTRUCTOR
   methods
      
       function obj = MockWaitbar(varargin)
           
       end
       
   end
   
   %% PUBLIC METHODS
   methods
      
       function close(obj) %#ok<MANU>
           
       end
       
   end
   
   %% STATIC METHODS
   methods (Static)
      
       function varargout = waitbar(varargin)
          
           obj = MockWaitbar(varargin{:});
           
           if nargout               
               varargout = {obj};
           end
           
       end
       
   end
   
end