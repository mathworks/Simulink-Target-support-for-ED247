classdef (SharedTestFixtures={ ...
        matlab.unittest.fixtures.PathFixture(fullfile(regexprep(mfilename('fullpath'),'\+.*',''),'_files')) ...
        }) ...
        MockedInterfaceTest < matlab.unittest.TestCase
    
    %% PRIVATE PROPERTIES
    properties (Access = private)
        currentdir_
        outputfolder_
    end
    
    %% CLASS SETUP
    methods (TestClassSetup)
        
        function skipTests(testCase)
            testCase.assumeTrue(false, 'Mock is not up-to-date')
        end
        
        function saveCurrentDirectory(testCase)
            testCase.currentdir_    = pwd;
            testCase.outputfolder_  = fullfile(tempdir,'ed247_MockedInterfaceTest');
        end
        
        function compileSFunction(testCase)
            
            clear('mex') %#ok<CLMEX>
            if ~isdir(testCase.outputfolder_)
                mkdir(testCase.outputfolder_)
            end
            cd(testCase.outputfolder_)
            ed247.compile('Mode','mock','Verbose',true,'OutputFolder',testCase.outputfolder_)
            
        end
        
        function disableIOWarnings(~)
            % Test models I/O are not connected
            warning('off','Simulink:Engine:InputNotConnected')
            warning('off','Simulink:Engine:OutputNotConnected')
        end
        
    end
    
    %% CLASS TEARDOWN
    methods (TestClassTeardown)
        
        function deleteMockedSFunction(testCase)
            clear('mex') %#ok<CLMEX>
            if isdir(testCase.outputfolder_)
                rmdir(testCase.outputfolder_,'s')
            end
        end
        
        function resetCurrentDirectory(testCase)
            cd(testCase.currentdir_)
        end
        
        function enableIOWarnings(~)
            warning('on','Simulink:Engine:InputNotConnected')
            warning('on','Simulink:Engine:OutputNotConnected')
        end
        
    end
    
    %% TESTS
    methods (Test)
        
        function testSimulationRun(testCase)
            
            % [ SETUP ]
            modelname = 'io_variable_number_harness';
            stoptime = 2;
            
            % [ EXERCISE ]
            f = @() evalc(sprintf('sim(''%s'',''StopTime'',''%d'')',modelname,stoptime)); % NOTE : EVALC is used to remove Command Window output (Mocked S-Function in Verbose mode)
            
            % [ VERIFY ]
            testCase.verifyWarningFree(f)
            
        end
        
        function testCustomize(testCase)
            
            % [ SETUP ]
            modelname = 'io_variable_number_harness';
            blockname = sprintf('%s/simple_sfun',modelname);
            
            filename = fullfile(tempdir(),'testCustomize.txt');
            fid = fopen(filename,'wt');
            fprintf(fid, '%s %s %s %d\n', 'In', 'data01', 'single', 2);
            fprintf(fid, '%s %s %s %d\n', 'In', 'data02', 'double', 4);
            fprintf(fid, '%s %s %s %d\n', 'In', 'data03', 'int8', 1);
            fprintf(fid, '%s %s %s %d\n', 'Out', 'data04', 'uint16', 3);
            fprintf(fid, '%s %s %s %d\n', 'Out', 'data05', 'int32', 1);
            fclose(fid);
            
            % [ EXERCISE ]
            load_system(modelname)
            set_param(blockname, 'filename', sprintf('''%s''', filename))
            evalc('sim(modelname)'); % NOTE : EVALC is used to remove Command Window output (Mocked S-Function in Verbose mode)
            
            % [ VERIFY ]
            ports = get_param(blockname,'PortHandles');
            testCase.verifyLength(ports.Inport,3)
            testCase.verifyLength(ports.Outport,2)
            
            % [ TEARDOWN ]
            delete(filename)
            bdclose(modelname)
            
        end
        
        function testInputAssignment(testCase)
            
            % [ SETUP ]
            modelname = 'mock2dev_in';
            
            % [ EXERCISE ]
            [log,simout] = evalc(sprintf('sim(''%s'',''StopTime'',''10'');', modelname));
            
            % [ VERIFY ]
            SIG_double = simout.get('logsout').get('SIG_double');
            expected = SIG_double.Values.Data;
            actual = regexp(log, sprintf('Input #%d<double>\\[0\\] = ([0-9.\\-]+)',SIG_double.PortIndex-1),'tokens');
            actual = str2double([actual{:}])';
            testCase.verifyEqual(actual,expected)
            
            SIG_single = simout.get('logsout').get('SIG_single');
            expected = SIG_single.Values.Data;
            actual = regexp(log, sprintf('Input #%d<single>\\[0\\] = ([0-9.\\-]+)',SIG_single.PortIndex-1),'tokens');
            actual = single(str2double([actual{:}]))';
            testCase.verifyEqual(actual,expected)
            
            SIG_int8 = simout.get('logsout').get('SIG_int8');
            expected = SIG_int8.Values.Data;
            actual = regexp(log, sprintf('Input #%d<int8>\\[0\\] = ([0-9.\\-]+)',SIG_int8.PortIndex-1),'tokens');
            actual = int8(str2double([actual{:}]))';
            testCase.verifyEqual(actual,expected)
            
            SIG_int16 = simout.get('logsout').get('SIG_int16');
            expected = SIG_int16.Values.Data;
            actual = regexp(log, sprintf('Input #%d<int16>\\[0\\] = ([0-9.\\-]+)',SIG_int16.PortIndex-1),'tokens');
            actual = int16(str2double([actual{:}]))';
            testCase.verifyEqual(actual,expected)
            
            SIG_int32 = simout.get('logsout').get('SIG_int32');
            expected = SIG_int32.Values.Data;
            actual = regexp(log, sprintf('Input #%d<int32>\\[0\\] = ([0-9.\\-]+)',SIG_int32.PortIndex-1),'tokens');
            actual = int32(str2double([actual{:}]))';
            testCase.verifyEqual(actual,expected)
            
            SIG_uint8 = simout.get('logsout').get('SIG_uint8');
            expected = SIG_uint8.Values.Data;
            actual = regexp(log, sprintf('Input #%d<uint8>\\[0\\] = ([0-9.\\-]+)',SIG_uint8.PortIndex-1),'tokens');
            actual = uint8(str2double([actual{:}]))';
            testCase.verifyEqual(actual,expected)
            
            SIG_uint16 = simout.get('logsout').get('SIG_uint16');
            expected = SIG_uint16.Values.Data;
            actual = regexp(log, sprintf('Input #%d<uint16>\\[0\\] = ([0-9.\\-]+)',SIG_uint16.PortIndex-1),'tokens');
            actual = uint16(str2double([actual{:}]))';
            testCase.verifyEqual(actual,expected)
            
            SIG_uint32 = simout.get('logsout').get('SIG_uint32');
            expected = SIG_uint32.Values.Data;
            actual = regexp(log, sprintf('Input #%d<uint32>\\[0\\] = ([0-9.\\-]+)',SIG_uint32.PortIndex-1),'tokens');
            actual = uint32(str2double([actual{:}]))';
            testCase.verifyEqual(actual,expected)
            
            SIG_boolean = simout.get('logsout').get('SIG_boolean');
            expected = SIG_boolean.Values.Data;
            actual = regexp(log, sprintf('Input #%d<boolean>\\[0\\] = ([0-9.\\-]+)',SIG_boolean.PortIndex-1),'tokens');
            actual = logical(str2double([actual{:}]))';
            testCase.verifyEqual(actual,expected)
            
        end
        
        function testOutputAssignment(testCase)
            
            % [ SETUP ]
            modelname = 'mock2dev_out';
            
            % [ EXERCISE ]
            [log,simout] = evalc(sprintf('sim(''%s'',''StopTime'',''10'');', modelname));
            
            % [ VERIFY ]
            SIG_double = simout.get('yout').get('SIG_double');
            expected = SIG_double.Values.Data;
            actual = regexp(log, sprintf('Output #%d<double>\\[0\\] = ([0-9.\\-]+)',0),'tokens');
            actual = str2double([actual{:}])';
            testCase.verifyEqual(actual,expected)
            
            SIG_single = simout.get('yout').get('SIG_single');
            expected = SIG_single.Values.Data;
            actual = regexp(log, sprintf('Output #%d<single>\\[0\\] = ([0-9.\\-]+)',1),'tokens');
            actual = single(str2double([actual{:}]))';
            testCase.verifyEqual(actual,expected)
            
            SIG_int8 = simout.get('yout').get('SIG_int8');
            expected = SIG_int8.Values.Data;
            actual = regexp(log, sprintf('Output #%d<int8>\\[0\\] = ([0-9.\\-]+)',2),'tokens');
            actual = int8(str2double([actual{:}]))';
            testCase.verifyEqual(actual,expected)
            
            SIG_int16 = simout.get('yout').get('SIG_int16');
            expected = SIG_int16.Values.Data;
            actual = regexp(log, sprintf('Output #%d<int16>\\[0\\] = ([0-9.\\-]+)',4),'tokens');
            actual = int16(str2double([actual{:}]))';
            testCase.verifyEqual(actual,expected)
            
            SIG_int32 = simout.get('yout').get('SIG_int32');
            expected = SIG_int32.Values.Data;
            actual = regexp(log, sprintf('Output #%d<int32>\\[0\\] = ([0-9.\\-]+)',6),'tokens');
            actual = int32(str2double([actual{:}]))';
            testCase.verifyEqual(actual,expected)
            
            SIG_uint8 = simout.get('yout').get('SIG_uint8');
            expected = SIG_uint8.Values.Data;
            actual = regexp(log, sprintf('Output #%d<uint8>\\[0\\] = ([0-9.\\-]+)',3),'tokens');
            actual = uint8(str2double([actual{:}]))';
            testCase.verifyEqual(actual,expected)
            
            SIG_uint16 = simout.get('yout').get('SIG_uint16');
            expected = SIG_uint16.Values.Data;
            actual = regexp(log, sprintf('Output #%d<uint16>\\[0\\] = ([0-9.\\-]+)',5),'tokens');
            actual = uint16(str2double([actual{:}]))';
            testCase.verifyEqual(actual,expected)
            
            SIG_uint32 = simout.get('yout').get('SIG_uint32');
            expected = SIG_uint32.Values.Data;
            actual = regexp(log, sprintf('Output #%d<uint32>\\[0\\] = ([0-9.\\-]+)',7),'tokens');
            actual = uint32(str2double([actual{:}]))';
            testCase.verifyEqual(actual,expected)
            
            SIG_boolean = simout.get('yout').get('SIG_boolean');
            expected = SIG_boolean.Values.Data;
            actual = regexp(log, sprintf('Output #%d<boolean>\\[0\\] = ([0-9.\\-]+)',8),'tokens');
            actual = logical(str2double([actual{:}]))';
            testCase.verifyEqual(actual,expected)
            
        end
        
        function testArrays(testCase)
            
            % [ SETUP ]
            modelname = 'mock2dev_arrays';
            
            % [ EXERCISE ]
            [log,simout] = evalc(sprintf('sim(''%s'',''StopTime'',''10'');', modelname));
            
            % [ VERIFY ]
            doubleArrayIN = simout.get('logsout').get('doubleArrayIN');
            expected = doubleArrayIN.Values.Data;
            col1 = regexp(log, sprintf('Input #%d<double>\\[%d\\] = ([0-9.\\-]+)',0,0),'tokens');
            col1 = str2double([col1{:}])';
            col2 = regexp(log, sprintf('Input #%d<double>\\[%d\\] = ([0-9.\\-]+)',0,1),'tokens');
            col2 = str2double([col2{:}])';
            col3 = regexp(log, sprintf('Input #%d<double>\\[%d\\] = ([0-9.\\-]+)',0,2),'tokens');
            col3 = str2double([col3{:}])';
            col4 = regexp(log, sprintf('Input #%d<double>\\[%d\\] = ([0-9.\\-]+)',0,3),'tokens');
            col4 = str2double([col4{:}])';
            col5 = regexp(log, sprintf('Input #%d<double>\\[%d\\] = ([0-9.\\-]+)',0,4),'tokens');
            col5 = str2double([col5{:}])';
            actual = [col1,col2,col3,col4,col5];
            testCase.verifyEqual(actual,expected, 'RelTol', 1e-3)
            
            uint8ArrayIN = simout.get('logsout').get('uint8ArrayIN');
            expected = uint8ArrayIN.Values.Data;
            col1 = regexp(log, sprintf('Input #%d<uint8>\\[%d\\] = ([0-9.\\-]+)',1,0),'tokens');
            col1 = str2double([col1{:}])';
            col2 = regexp(log, sprintf('Input #%d<uint8>\\[%d\\] = ([0-9.\\-]+)',1,1),'tokens');
            col2 = str2double([col2{:}])';
            col3 = regexp(log, sprintf('Input #%d<uint8>\\[%d\\] = ([0-9.\\-]+)',1,2),'tokens');
            col3 = str2double([col3{:}])';
            actual = uint8([col1,col2,col3]);
            testCase.verifyEqual(actual,expected, 'RelTol', 1e-3)
            
            uint16ArrayOUT = simout.get('yout').get('uint16ArrayOUT');
            expected = uint16ArrayOUT.Values.Data;
            col1 = regexp(log, sprintf('Output #%d<uint16>\\[%d\\] = ([0-9.\\-]+)',0,0),'tokens');
            col1 = str2double([col1{:}])';
            col2 = regexp(log, sprintf('Output #%d<uint16>\\[%d\\] = ([0-9.\\-]+)',0,1),'tokens');
            col2 = str2double([col2{:}])';
            actual = uint16([col1,col2]);
            testCase.verifyEqual(actual,expected, 'RelTol', 1e-3)
            
        end
        
        function testMultiDimensionsArrays(testCase)
            
            % [ SETUP ]
            modelname = 'mock2dev_multidimarrays';
            
            t = (0:9)';
            multidimarraysinputdataset = Simulink.SimulationData.Dataset;
            multidimarraysinputdataset = multidimarraysinputdataset.addElement(timeseries(int16(1:10)',                                 t, 'Name','int16ScalarIN'));
            multidimarraysinputdataset = multidimarraysinputdataset.addElement(timeseries(int8(reshape((1:(2*2*10)),2,2,10)),           t, 'Name','int8Mat2x2IN'));
            multidimarraysinputdataset = multidimarraysinputdataset.addElement(timeseries(single(reshape((1:(1*2*10))*0.1,1,2,10)),     t, 'Name','singleMat1x2IN'));
            multidimarraysinputdataset = multidimarraysinputdataset.addElement(timeseries(int16(reshape((1:(1*2*3*10)),1,2,3,10)) + 500,t, 'Name','int16Arr1x2x3IN'));
            multidimarraysinputdataset = multidimarraysinputdataset.addElement(timeseries(int8(reshape((1:(2*1*10)),2,1,10)) * -1,      t, 'Name','int8Mat2x1IN'));
            multidimarraysinputdataset = multidimarraysinputdataset.addElement(timeseries(single(reshape((1:(1*4*10))*-0.1,1,4,10)),    t, 'Name','singleMat1x4IN'));
            assignin('base','multidimarraysinputdataset',multidimarraysinputdataset)
            
            % [ EXERCISE ]
            [log,simout] = evalc(sprintf('sim(''%s'',''StopTime'',''10'',''LoadExternalInput'',''on'',''ExternalInput'',''multidimarraysinputdataset'');', modelname));
                        
            % [ VERIFY ]
            int16ScalarIN = simout.get('logsout').get('int16ScalarIN');
            expected = int16ScalarIN.Values.Data;
            col1 = regexp(log, sprintf('Input #%d<int16>\\[%d\\] = ([0-9.\\-]+)',0,0),'tokens');
            col1 = str2double([col1{:}])';
            actual = int16(col1);
            testCase.verifyEqual(actual,expected, 'RelTol', 1e-3)
                        
            int8Mat2x2IN = simout.get('logsout').get('int8Mat2x2IN');
            expected = int8Mat2x2IN.Values.Data;
            actual = zeros(size(expected),'int8');
            col = regexp(log, sprintf('Input #%d<int8>\\[%d\\] = ([0-9.\\-]+)',1,0),'tokens');
            actual(1,1,:) = str2double([col{:}]);
            col = regexp(log, sprintf('Input #%d<int8>\\[%d\\] = ([0-9.\\-]+)',1,2),'tokens');
            actual(1,2,:) = str2double([col{:}]);
            col = regexp(log, sprintf('Input #%d<int8>\\[%d\\] = ([0-9.\\-]+)',1,1),'tokens');
            actual(2,1,:) = str2double([col{:}]);
            col = regexp(log, sprintf('Input #%d<int8>\\[%d\\] = ([0-9.\\-]+)',1,3),'tokens');
            actual(2,2,:) = str2double([col{:}]);
            testCase.verifyEqual(actual,expected, 'RelTol', 1e-3)
                        
            singleMat1x2IN = simout.get('logsout').get('singleMat1x2IN');
            expected = singleMat1x2IN.Values.Data;
            actual = zeros(size(expected),'single');
            col = regexp(log, sprintf('Input #%d<single>\\[%d\\] = ([0-9.\\-]+)',2,0),'tokens');
            actual(1,1,:) = str2double([col{:}]);
            col = regexp(log, sprintf('Input #%d<single>\\[%d\\] = ([0-9.\\-]+)',2,1),'tokens');
            actual(1,2,:) = str2double([col{:}]);            
            testCase.verifyEqual(actual,expected, 'RelTol', 1e-3)
            
            int16Arr1x2x3IN = simout.get('logsout').get('int16Arr1x2x3IN');
            expected = int16Arr1x2x3IN.Values.Data;
            actual = zeros(size(expected),'int16');
            col = regexp(log, sprintf('Input #%d<int16>\\[%d\\] = ([0-9.\\-]+)',3,0),'tokens');
            actual(1,1,1,:) = str2double([col{:}]);
            col = regexp(log, sprintf('Input #%d<int16>\\[%d\\] = ([0-9.\\-]+)',3,2),'tokens');
            actual(1,1,2,:) = str2double([col{:}]);  
            col = regexp(log, sprintf('Input #%d<int16>\\[%d\\] = ([0-9.\\-]+)',3,4),'tokens');
            actual(1,1,3,:) = str2double([col{:}]); 
            col = regexp(log, sprintf('Input #%d<int16>\\[%d\\] = ([0-9.\\-]+)',3,1),'tokens');
            actual(1,2,1,:) = str2double([col{:}]);
            col = regexp(log, sprintf('Input #%d<int16>\\[%d\\] = ([0-9.\\-]+)',3,3),'tokens');
            actual(1,2,2,:) = str2double([col{:}]);  
            col = regexp(log, sprintf('Input #%d<int16>\\[%d\\] = ([0-9.\\-]+)',3,5),'tokens');
            actual(1,2,3,:) = str2double([col{:}]);             
            testCase.verifyEqual(actual,expected, 'RelTol', 1e-3)
            
            int8Mat2x1IN = simout.get('logsout').get('int8Mat2x1IN');
            expected = int8Mat2x1IN.Values.Data;
            actual = zeros(size(expected),'int8');
            col = regexp(log, sprintf('Input #%d<int8>\\[%d\\] = ([0-9.\\-]+)',4,0),'tokens');
            actual(1,1,:) = str2double([col{:}]);
            col = regexp(log, sprintf('Input #%d<int8>\\[%d\\] = ([0-9.\\-]+)',4,1),'tokens');
            actual(2,1,:) = str2double([col{:}]);
            testCase.verifyEqual(actual,expected, 'RelTol', 1e-3)
                        
            singleMat1x4IN = simout.get('logsout').get('singleMat1x4IN');
            expected = singleMat1x4IN.Values.Data;
            actual = zeros(size(expected),'single');
            col = regexp(log, sprintf('Input #%d<single>\\[%d\\] = ([0-9.\\-]+)',5,0),'tokens');
            actual(1,1,:) = str2double([col{:}]);
            col = regexp(log, sprintf('Input #%d<single>\\[%d\\] = ([0-9.\\-]+)',5,1),'tokens');
            actual(1,2,:) = str2double([col{:}]);
            col = regexp(log, sprintf('Input #%d<single>\\[%d\\] = ([0-9.\\-]+)',5,2),'tokens');
            actual(1,3,:) = str2double([col{:}]);   
            col = regexp(log, sprintf('Input #%d<single>\\[%d\\] = ([0-9.\\-]+)',5,3),'tokens');
            actual(1,4,:) = str2double([col{:}]);   
            testCase.verifyEqual(double(actual),double(expected), 'RelTol', 1e-3)
            
            int16Vec3OUT = simout.get('yout').get('int16Vec3OUT');
            expected = int16Vec3OUT.Values.Data;
            actual = zeros(size(expected),'int16');
            col = regexp(log, sprintf('Output #%d<int16>\\[%d\\] = ([0-9.\\-]+)',0,0),'tokens');
            actual(:,1) = str2double([col{:}]);
            col = regexp(log, sprintf('Output #%d<int16>\\[%d\\] = ([0-9.\\-]+)',0,1),'tokens');
            actual(:,2) = str2double([col{:}]);
            col = regexp(log, sprintf('Output #%d<int16>\\[%d\\] = ([0-9.\\-]+)',0,2),'tokens');
            actual(:,3) = str2double([col{:}]);
            testCase.verifyEqual(actual,expected)
            
            int32Mat4x2OUT = simout.get('yout').get('int32Mat4x2OUT');
            expected = int32Mat4x2OUT.Values.Data;
            actual = zeros(size(expected),'int32');
            col = regexp(log, sprintf('Output #%d<int32>\\[%d\\] = ([0-9.\\-]+)',1,0),'tokens');
            actual(1,1,:) = str2double([col{:}]);
            col = regexp(log, sprintf('Output #%d<int32>\\[%d\\] = ([0-9.\\-]+)',1,1),'tokens');
            actual(2,1,:) = str2double([col{:}]);
            col = regexp(log, sprintf('Output #%d<int32>\\[%d\\] = ([0-9.\\-]+)',1,2),'tokens');
            actual(3,1,:) = str2double([col{:}]);
            col = regexp(log, sprintf('Output #%d<int32>\\[%d\\] = ([0-9.\\-]+)',1,3),'tokens');
            actual(4,1,:) = str2double([col{:}]);
            col = regexp(log, sprintf('Output #%d<int32>\\[%d\\] = ([0-9.\\-]+)',1,4),'tokens');
            actual(1,2,:) = str2double([col{:}]);
            col = regexp(log, sprintf('Output #%d<int32>\\[%d\\] = ([0-9.\\-]+)',1,5),'tokens');
            actual(2,2,:) = str2double([col{:}]);
            col = regexp(log, sprintf('Output #%d<int32>\\[%d\\] = ([0-9.\\-]+)',1,6),'tokens');
            actual(3,2,:) = str2double([col{:}]);
            col = regexp(log, sprintf('Output #%d<int32>\\[%d\\] = ([0-9.\\-]+)',1,7),'tokens');
            actual(4,2,:) = str2double([col{:}]);            
            testCase.verifyEqual(actual,expected)
            
            int16Arr3x1x2OUT = simout.get('yout').get('int16Arr3x1x2OUT');
            expected = int16Arr3x1x2OUT.Values.Data;
            actual = zeros(size(expected),'int16');
            col = regexp(log, sprintf('Output #%d<int16>\\[%d\\] = ([0-9.\\-]+)',2,0),'tokens');
            actual(1,1,1,:) = str2double([col{:}]);
            col = regexp(log, sprintf('Output #%d<int16>\\[%d\\] = ([0-9.\\-]+)',2,1),'tokens');
            actual(2,1,1,:) = str2double([col{:}]);
            col = regexp(log, sprintf('Output #%d<int16>\\[%d\\] = ([0-9.\\-]+)',2,2),'tokens');
            actual(3,1,1,:) = str2double([col{:}]);
            col = regexp(log, sprintf('Output #%d<int16>\\[%d\\] = ([0-9.\\-]+)',2,3),'tokens');
            actual(1,1,2,:) = str2double([col{:}]);
            col = regexp(log, sprintf('Output #%d<int16>\\[%d\\] = ([0-9.\\-]+)',2,4),'tokens');
            actual(2,1,2,:) = str2double([col{:}]);
            col = regexp(log, sprintf('Output #%d<int16>\\[%d\\] = ([0-9.\\-]+)',2,5),'tokens');
            actual(3,1,2,:) = str2double([col{:}]);
            testCase.verifyEqual(actual,expected)
            
            int32Vec4OUT = simout.get('yout').get('int32Vec4OUT');
            expected = int32Vec4OUT.Values.Data;
            actual = zeros(size(expected),'int32');
            col = regexp(log, sprintf('Output #%d<int32>\\[%d\\] = ([0-9.\\-]+)',3,0),'tokens');
            actual(:,1) = str2double([col{:}]);
            col = regexp(log, sprintf('Output #%d<int32>\\[%d\\] = ([0-9.\\-]+)',3,1),'tokens');
            actual(:,2) = str2double([col{:}]);
            col = regexp(log, sprintf('Output #%d<int32>\\[%d\\] = ([0-9.\\-]+)',3,2),'tokens');
            actual(:,3) = str2double([col{:}]);
            col = regexp(log, sprintf('Output #%d<int32>\\[%d\\] = ([0-9.\\-]+)',3,3),'tokens');
            actual(:,4) = str2double([col{:}]);
            testCase.verifyEqual(actual,expected)
            
        end
        
    end
    
end