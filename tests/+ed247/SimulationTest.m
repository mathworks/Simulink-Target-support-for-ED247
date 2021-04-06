classdef (SharedTestFixtures={ ...
        matlab.unittest.fixtures.PathFixture(fullfile(regexprep(mfilename('fullpath'),'\+.*',''),'_files')), ...
        matlab.unittest.fixtures.PathFixture(fullfile(regexprep(mfilename('fullpath'),'\+.*',''),'_files','ecic')), ...
        }) ...
        SimulationTest < matlab.unittest.TestCase
    
    %% CLASS SETUP
    methods (TestClassSetup)
        
        function verifySFunction(testCase)            
            testCase.assertEqual(exist('ed247_sfun','file'),3, ...
                'MEX file for S-Function "ed247_sfun" does not exist')            
        end
        
        function startPool(testCase)
            testCase.pool_ = parpool('local',2);
        end
        
    end
    
    %% CLASS TEARDOWN
    methods (TestClassTeardown)
       
        function closePool(testCase)
           delete(testCase.pool_) 
        end
        
    end
    
    %% TESTS
    methods (Test)
        
        function testNAD(testCase)
           
            testCase.assumeFalse(verLessThan('MATLAB','9.2'), ...
                'Simulink.SimulationInput is not defined for MATLAB releases lower than 9.2 (r2017a)')
            
            % [ SETUP ]            
            in = [ ...
                Simulink.SimulationInput(['send_nad_r',version('-release')]); ...
                Simulink.SimulationInput(['receive_nad_r',version('-release')]); ...
                ];
            
            ds = Simulink.SimulationData.Dataset();
            
            ds = ds.addElement(timeseries(uint8([0;255]), [0;10], 'Name', 'input01'), 'Name', 'input01');
            ds = ds.addElement(timeseries(uint8([255;0]), [0;10], 'Name', 'input02'), 'Name', 'input02');
            ds = ds.addElement(timeseries(uint8(abs([-255;255])), [0;10], 'Name', 'input03'), 'Name', 'input03');            
            ds = ds.addElement(timeseries(single(reshape(0:10,1,1,[]) .* [1,2;3,4]), (0:10)', 'Name', 'input04'), 'Name', 'input04');
            in(1) = in(1).setExternalInput(ds);
            
            % [ EXERCISE ]            
            parfor i = 1:numel(in)
                out(i) = sim(in(i)); %#ok<SIM>
            end
            
            % [ VERIFY ]
            input  = out(1).logsout;
            output = out(2).yout;
            
            input01   = input.get('input01').Values.Data;
            input02   = input.get('input02').Values.Data;
            output01  = output.get('output01').Values.Data;
            output02  = output.get('output02').Values.Data;
            
            testCase.verifyTrue(all(ismember(output01,input01)))
            testCase.verifyTrue(all(ismember(output02,input02)))
                        
        end
        
    end
    
    %% PRIVATE PROPERTIES
    properties (Access = private)
        pool_
    end
    
end