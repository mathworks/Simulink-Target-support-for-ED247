classdef (SharedTestFixtures={ ...
        matlab.unittest.fixtures.PathFixture(fullfile(regexprep(mfilename('fullpath'),'\+.*',''),'_files')), ...
        matlab.unittest.fixtures.PathFixture(fullfile(regexprep(mfilename('fullpath'),'\+.*',''),'_files','ecic')), ...
        tools.ParallelPoolFixture('local',2), ...
        }) ...
        SimulationTest < matlab.unittest.TestCase
    
    %% CLASS SETUP
    methods (TestClassSetup)
        
        function checkParallelComputingToolbox(testCase)
            testCase.assumeNotEmpty(which("gcp"), "Parallel Computing Toolbox is required to run these tests (not installed)")
        end
        
        function verifySFunction(testCase)
            testCase.assertEqual(exist('ed247_sfun','file'),3, ...
                'MEX file for S-Function "ed247_sfun" does not exist')
        end
        
    end
    
    %% TESTS
    methods (Test)
        
        function testNAD(testCase)
            
            testCase.assumeFalse(verLessThan('MATLAB','9.2'), ...
                'Simulink.SimulationInput is not defined for MATLAB releases lower than 9.2 (r2017a)')
            
            % [ SETUP ]
            sendmodel       = ['send_nad_r',version('-release')];
            receivemodel    = ['receive_nad_r',version('-release')];
            closeModelSend  = onCleanup(@() bdclose(sendmodel));
            closeModelRecv  = onCleanup(@() bdclose(receivemodel));
            
            testCase.assumeEqual(exist(sendmodel,'file'),4, ...
                sprintf('Model "%s" is not available', sendmodel))
            testCase.assumeEqual(exist(receivemodel,'file'),4, ...
                sprintf('Model "%s" is not available', receivemodel))
            
            in = [ ...
                Simulink.SimulationInput(sendmodel); ...
                Simulink.SimulationInput(receivemodel); ...
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
            
            % Signal #1           
            output01 = output.get('Signal00').Values.Data;
            output01 = unique(output01,'stable');
            output01 = output01(find(output01 == 0, 1, 'last')+1:end);
            input01  = input.get('input01').Values.Data;
            input01  = input01(find(input01 > 0, 1, 'first'):end);
            input01(length(output01)+1:end) = [];
            
            testCase.verifyEqual(output01,input01, 'Output01 (Signal00) should match Input01')
            
            % Signal #2
            output02 = output.get('Signal01').Values.Data;
            output02 = unique(output02,'stable');
            output02 = output02(find(output02 > 0, 1, 'first'):end);            
            input02  = input.get('input02').Values.Data;
            input02  = input02(find(input02 > 0, 1, 'first'):end); 
            input02(length(output02)+1:end) = [];
                        
            testCase.verifyEqual(output02,input02, 'Output02 (Signal01) should match Input02')
            
        end
        
        function testA429(testCase)
            
            testCase.assumeFalse(verLessThan('MATLAB','9.2'), ...
                'Simulink.SimulationInput is not defined for MATLAB releases lower than 9.2 (r2017a)')
            
            % [ SETUP ]
            sendmodel       = ['send_a429_r',version('-release')];
            receivemodel    = ['receive_a429_r',version('-release')];
            closeModelSend  = onCleanup(@() bdclose(sendmodel));
            closeModelRecv  = onCleanup(@() bdclose(receivemodel));
            
            testCase.assumeEqual(exist(sendmodel,'file'),4, ...
                sprintf('Model "%s" is not available', sendmodel))
            testCase.assumeEqual(exist(receivemodel,'file'),4, ...
                sprintf('Model "%s" is not available', receivemodel))
            
            in = [ ...
                Simulink.SimulationInput(sendmodel); ...
                Simulink.SimulationInput(receivemodel); ...
                ];
            
            ds = Simulink.SimulationData.Dataset();
                        
            ds = ds.addElement(timeseries(uint8((0:100)' + (1:4)),      (0:0.1:10)', 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_350_10'),          'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_350_10');
            ds = ds.addElement(timeseries(true(2,1),                    [0;10],      'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_350_10_refresh'),  'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_350_10_refresh');
            ds = ds.addElement(timeseries(uint8((255:-1:155)' + (1:4)), (0:0.1:10)', 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_200_10'),          'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_200_10');
            ds = ds.addElement(timeseries(true(2,1),                    [0;10],      'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_200_10_refresh'),  'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_200_10_refresh');            
            ds = ds.addElement(timeseries(uint8((150:250)' + (1:4)),    (0:0.1:10)', 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_200_11'),          'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_200_11');
            ds = ds.addElement(timeseries(true(2,1),                    [0;10],      'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_200_11_refresh'),  'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_200_11_refresh');
            in(1) = in(1).setExternalInput(ds);
            
            % [ EXERCISE ]
            parfor i = 1:numel(in)
                out(i) = sim(in(i)); %#ok<SIM>
            end
            
            % [ VERIFY ]
            input  = out(1).logsout;
            output = out(2).yout;
            
            %
            % T11M4_A429_SIMU2SWIM_BUS_1_350_10
            %
            name = 'T11M4_A429_SIMU2SWIM_BUS_1_350_10';
            in    = input.get(name).Values.Data;
            out   = output.get(name).Values.Data;
            
            nodata = all(out == 0,2);
            out(nodata,:) = [];
            testCase.assertNotEmpty(out, sprintf('No data received for signal "%s"', name))
            compare = cumsum(all(in == out(1,:),2)) == 1;
            sendmessage = in(compare,:);
            recvmessage = out(1:min([end,size(sendmessage,1)]),:);
            sendmessage(size(recvmessage,1)+1:end,:) = [];
            
            testCase.verifyNotEmpty(recvmessage, 'No data received')
            % Convert signals to double to allow usage of AbsTol (ignored
            % for UINT8)
            testCase.verifyEqual(double(recvmessage),double(sendmessage),'AbsTol',1, ...
                sprintf('Received data for %s does not match send data',name))
            
            %
            % T11M4_A429_SIMU2SWIM_BUS_1_200_10
            %
            name = 'T11M4_A429_SIMU2SWIM_BUS_1_200_10';
            in   = input.get(name).Values.Data;
            out  = output.get(name).Values.Data;
            
            nodata = all(out == 0,2);
            out(nodata,:) = [];
            testCase.assertNotEmpty(out, sprintf('No data received for signal "%s"', name))
            compare = cumsum(all(in == out(1,:),2)) == 1;
            sendmessage = in(compare,:);
            recvmessage = out(1:min([end,size(sendmessage,1)]),:);
            sendmessage(size(recvmessage,1)+1:end,:) = [];
            
            testCase.verifyNotEmpty(recvmessage, 'No data received')
            % Convert signals to double to allow usage of AbsTol (ignored
            % for UINT8)
            testCase.verifyEqual(double(recvmessage),double(sendmessage),'AbsTol',1, ...
                sprintf('Received data for %s does not match send data',name))
            
            %
            % T11M4_A429_SIMU2SWIM_BUS_1_200_11
            %
            name = 'T11M4_A429_SIMU2SWIM_BUS_1_200_11';
            in   = input.get(name).Values.Data;
            out  = output.get(name).Values.Data;
            
            nodata = all(out == 0,2);
            out(nodata,:) = [];
            testCase.assertNotEmpty(out, sprintf('No data received for signal "%s"', name))
            compare = cumsum(all(in == out(1,:),2)) == 1;
            sendmessage = in(compare,:);
            recvmessage = out(1:min([end,size(sendmessage,1)]),:);
            sendmessage(size(recvmessage,1)+1:end,:) = [];
            
            testCase.verifyNotEmpty(recvmessage, 'No data received')
            % Convert signals to double to allow usage of AbsTol (ignored
            % for UINT8)
            testCase.verifyEqual(double(recvmessage),double(sendmessage),'AbsTol',1, ...
                sprintf('Received data for %s does not match send data',name))
            
        end
        
        function testA664(testCase)
            
            testCase.assumeTrue(false, "TODO: Not working yet")
            testCase.assumeFalse(verLessThan('MATLAB','9.2'), ...
                'Simulink.SimulationInput is not defined for MATLAB releases lower than 9.2 (r2017a)')
            
            % [ SETUP ]
            sendmodel       = ['send_a664_r',version('-release')];
            receivemodel    = ['receive_a664_r',version('-release')];
            closeModelSend  = onCleanup(@() bdclose(sendmodel));
            closeModelRecv  = onCleanup(@() bdclose(receivemodel));
            
            testCase.assumeEqual(exist(sendmodel,'file'),4, ...
                sprintf('Model "%s" is not available', sendmodel))
            testCase.assumeEqual(exist(receivemodel,'file'),4, ...
                sprintf('Model "%s" is not available', receivemodel))
            
            in = [ ...
                Simulink.SimulationInput(sendmodel); ...
                Simulink.SimulationInput(receivemodel); ...
                ];
            
            ds = Simulink.SimulationData.Dataset();
                        
            ds = ds.addElement(timeseries(true(2,1),                [0;10],     'Name', 'T11MX_19492_WAIT_STEP_refresh'),   'Name', 'T11MX_19492_WAIT_STEP_refresh');
            ds = ds.addElement(timeseries(uint8((0:100)' + (1:4)), (0:0.1:10)', 'Name', 'T11MX_19492_WAIT_STEP_I'),         'Name', 'T11MX_19492_WAIT_STEP_I');
            in(1) = in(1).setExternalInput(ds);
            
            % [ EXERCISE ]
            parfor i = 1:numel(in)
                out(i) = sim(in(i)); %#ok<SIM>
            end
            
            % [ VERIFY ]
            input  = out(1).logsout;
            output = out(2).yout;
            
            %
            % T11MX_19492_WAIT_STEP_I and T11MX_15430_TEST_STEP_O
            %
            inname  = 'T11MX_19492_WAIT_STEP_I';
            outname = 'T11MX_15430_TEST_STEP_O';
            in    = input.get(inname).Values.Data;
            out   = output.get(outname).Values.Data;
            
            nodata = all(out == 0,2);
            out(nodata,:) = [];
            testCase.assertNotEmpty(out, 'No data received')
            
            compare = cumsum(all(in == out(1,:),2)) == 1;
            sendmessage = in(compare,:);
            recvmessage = out(1:min([end,size(sendmessage,1)]),:);
            sendmessage(size(recvmessage,1)+1:end,:) = [];
            
            testCase.verifyEqual(recvmessage,sendmessage,'AbsTol',1, ...
                sprintf('Received data for %s (in) and %s (out) does not match send data',inname,outname))
            
        end
        
        function testA825(testCase)
            
            testCase.assumeFalse(verLessThan('MATLAB','9.2'), ...
                'Simulink.SimulationInput is not defined for MATLAB releases lower than 9.2 (r2017a)')
            
            % [ SETUP ]
            sendmodel       = ['send_a825_r',version('-release')];
            receivemodel    = ['receive_a825_r',version('-release')];
            closeModelSend  = onCleanup(@() bdclose(sendmodel));
            closeModelRecv  = onCleanup(@() bdclose(receivemodel));
            
            testCase.assumeEqual(exist(sendmodel,'file'),4, ...
                sprintf('Model "%s" is not available', sendmodel))
            testCase.assumeEqual(exist(receivemodel,'file'),4, ...
                sprintf('Model "%s" is not available', receivemodel))
            
            in = [ ...
                Simulink.SimulationInput(sendmodel); ...
                Simulink.SimulationInput(receivemodel); ...
                ];
            
            ds = Simulink.SimulationData.Dataset();
                        
            ds = ds.addElement(timeseries(uint8((0:100)' + (1:3)),  (0:0.1:10)', 'Name', 'Stream0'),            'Name', 'Stream0');
            ds = ds.addElement(timeseries(true(2,1),                [0;10],      'Name', 'Stream0_refresh'),    'Name', 'Stream0_refresh');
            ds = ds.addElement(timeseries(uint8((0:100)' + (1:5)),  (0:0.1:10)', 'Name', 'Stream1'),            'Name', 'Stream1');
            ds = ds.addElement(timeseries(true(2,1),                [0;10],      'Name', 'Stream1_refresh'),    'Name', 'Stream1_refresh');
            in(1) = in(1).setExternalInput(ds);
            
            % [ EXERCISE ]
            parfor i = 1:numel(in)
                out(i) = sim(in(i)); %#ok<SIM>
            end
            
            % [ VERIFY ]
            input  = out(1).logsout;
            output = out(2).yout;
            
            %
            % Stream0
            %
            inname  = 'Stream0';
            outname = 'Stream0';
            in    = input.get(inname).Values.Data;
            out   = output.get(outname).Values.Data;
            
            nodata = all(out == 0,2);
            out(nodata,:) = [];
            testCase.assertNotEmpty(out, 'No data received')
            
            compare = cumsum(all(in == out(1,:),2)) == 1;
            sendmessage = in(compare,:);
            recvmessage = out(1:min([end,size(sendmessage,1)]),:);
            sendmessage(size(recvmessage,1)+1:end,:) = [];
            
            testCase.verifyEqual(recvmessage,sendmessage,'AbsTol',1, ...
                sprintf('Received data for %s (in) and %s (out) does not match send data',inname,outname))
            
        end
        
    end
    
    %% PRIVATE PROPERTIES
    properties (Access = private)
        pool_
    end
    
end