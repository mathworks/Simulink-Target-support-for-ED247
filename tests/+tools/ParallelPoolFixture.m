classdef ParallelPoolFixture < matlab.unittest.fixtures.Fixture
    
    %% PRIVATE PROPERTIES
    properties (Access=private)
        pool_
    end
    
    %% IMMUTABLE PROPERTIES
    properties (SetAccess = immutable, GetAccess = private)
        name_
        nworkers_
    end
    
    %% CONSTRUCTOR
    methods
        
        function fixture = ParallelPoolFixture(name,nworkers)
            validateattributes(name, {'char'}, {'row'})
            validateattributes(nworkers, {'double'}, {'scalar'})
            fixture.name_ = name;
            fixture.nworkers_ = nworkers;
            fixture.SetupDescription = sprintf( ...
                'Set the parallel pool name to "%s".\nSet the number of workers to %d\n',...
                fixture.name_, fixture.nworkers_);
        end
        
    end
    
    %% SETUP / TEARDOWN
    methods
        
        function setup(fixture)
            fixture.pool_ = parpool('local',2);
        end
        
        function teardown(fixture)
            delete(fixture.pool_)
        end
        
    end
    
    %% FIXTURE INTERFACE
    methods (Access=protected)
        
        function bool = isCompatible(fixture, other)
            bool = strcmp(fixture.name_, other.name_);
        end
        
    end
end