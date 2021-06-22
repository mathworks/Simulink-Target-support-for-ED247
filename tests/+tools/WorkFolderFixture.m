classdef WorkFolderFixture < matlab.unittest.fixtures.Fixture
    % WORKFOLDERFIXTURE
    %
    % Copyright 2021, The MathWorks Inc.
    %
    
    %% PRIVATE PROPERTIES
    properties (Access = private)
        currentfolder_
        workfolder_
    end
    
    %% IMMUTABLE PROPERTIES
    properties (SetAccess = immutable, GetAccess = private)
        name_
    end
    
    %% CONSTRUCTOR
    methods
       
        function fixture = WorkFolderFixture(name,varargin)
            fixture.name_ = name;
        end
        
    end
    
    %% FIXTURE METHODS
    methods
       
        function setup(fixture)
            
            setWorkFolder(fixture)
            saveCurrentFolder(fixture)
            setupWorkFolder(fixture)
            changeCurrentDirectory(fixture)
            
        end
        
        function teardown(fixture)
            
            resetCurrentDirectory(fixture)
            cleanupWorkFolder(fixture)
            
        end
        
    end
    
    %% FIXTURE PROTECTED METHODS
    methods (Access=protected)
        
        function bool = isCompatible(fixture, other)
            bool = isequal(fixture.name_, other.name_);
        end
        
    end
    
    %% PROTECTED METHODS
    methods (Access = protected)
        
        function setWorkFolder(fixture)
            fixture.workfolder_ = fullfile(tempdir(),fixture.name_{:});
        end
        
        function saveCurrentFolder(fixture)
            
            if strcmp(pwd,fixture.workfolder_)
                proj = currentProject();
                fixture.currentfolder_ = proj.RootFolder;
            else
                fixture.currentfolder_ = pwd;
            end
            
        end
                
        function setupWorkFolder(fixture)
            
            cleanupWorkFolder(fixture)
            mkdir(fixture.workfolder_)            
            
        end
        
        function changeCurrentDirectory(fixture)
            cd(fixture.workfolder_)
        end
                
        function cleanupWorkFolder(fixture)
            
            if strcmp(fixture.workfolder_,pwd)
                cd(fixture.currentfolder_)
            end
            
            if isdir(fixture.workfolder_) %#ok<ISDIR> Backward compatibility
                try rmdir(fixture.workfolder_,'s'); end %#ok<TRYNC>
            end
            
        end
        
        function resetCurrentDirectory(fixture)
            cd(fixture.currentfolder_)
        end
        
    end
    
end