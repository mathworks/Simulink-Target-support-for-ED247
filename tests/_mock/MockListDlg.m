classdef MockListDlg < matlab.mixin.SetGet
   % MOCKLISTDLG Mock the WAITBAR function for testing purpose
   
   %% PRIVATE PROPERTIES
   properties (Access = private)
       outputs_
   end
   
   %% CONSTRUCTOR
   methods
      
       function obj = MockListDlg(varargin)
           obj.outputs_ = varargin;
       end
       
   end
   
   %% PUBLIC METHODS
   methods
       
       function varargout = listdlg(obj,varargin)
           
           if nargout
               
               if ~isempty(obj.outputs_)
                   varargout = [obj.outputs_(1), {~isempty(obj.outputs_{1})}];
                   obj.outputs_(1) = [];
               else
                   varargout = {[], false};
               end
               
           end
           
       end
       
   end
   
end