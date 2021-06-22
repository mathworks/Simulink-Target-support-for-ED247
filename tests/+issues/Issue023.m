classdef (SharedTestFixtures={...
        matlab.unittest.fixtures.PathFixture(fullfile(regexprep(mfilename('fullpath'),'\+.*',''),'_mock')), ...
        tools.WorkFolderFixture({'ED247','Issue023'})}) ...
        Issue023 < matlab.unittest.TestCase
    % ISSUE023
    %
    % Create helper to connect signals
    %
    
    %% CLASS SETUP
    methods (TestClassSetup)
        
        function setFileFolder(testCase)
            proj = currentProject();
            testCase.filefolder_ = fullfile(proj.RootFolder,'tests','_files');
        end
        
    end
    
    %% TESTS
    methods (Test)
        
        function testSimpleA429ReceiveWithoutRefresh(testCase)
            
            % [ SETUP ]
            archivename = fullfile(testCase.filefolder_, 'A429_simple01.zip');
            unzip(archivename,pwd)
            
            modelname = 'readsimplea429';
            new_system(modelname)
            load_system(modelname)
            closeModel = onCleanup(@() bdclose(modelname));
            
            %
            % Add blocks in the model
            %
            configurationblockname = [modelname,'/Configure'];
            configurationblock = add_block('lib_ed247/ED247_Configuration', configurationblockname);
            
            receiveblockname = [modelname,'/Receive'];
            receiveblock = add_block('lib_ed247/ED247_Receive', receiveblockname);
                        
            %
            % Configure blocks
            %   - configuration file
            %   - Enable refresh
            %
            set(configurationblock, 'configurationFilename', '''a429_01_ecic_in.xml''')
            set(receiveblock, 'enable_refresh', 'off')
            
            % Run SIM to update diagram only (do not care about warnings)
            warning('off')
            sim(modelname,'StopTime','0');
            warning('on')
            
            receiveobj = ed247.blocks.Receive(receiveblock);
            receiveobj.fWaitbar = @(varargin) MockWaitbar.waitbar(varargin{:});
            
            % [ EXERCISE ]
            createBus(receiveobj)
            
            % [ VERIFY ]
            receivesubsysblock = find_system(modelname,'FindAll','on','SearchDepth',1,'Type','block','Name','Receive');
            testCase.assertLength(receivesubsysblock, 1, ...
                sprintf('Model should contain 1 block named %s', 'Receive'))
            testCase.assertEqual(get(receivesubsysblock,'BlockType'), 'SubSystem', ...
                sprintf('Block %s should be a subsystem', 'Receive'))
            
            ports = get(receivesubsysblock,'PortHandles');
            testCase.verifyEmpty(ports.Inport, 'Receive subsystem should not have any inport')
            testCase.verifyLength(ports.Outport, 1, 'Receive subsystem should have 1 outport')
            
            busselector = add_block('simulink/Signal Routing/Bus Selector',[modelname,'/bs']);
            pos = get(receivesubsysblock,'Position');
            set(busselector, 'Position', pos + [100,0,100,0])
            add_line(modelname,'Receive/1','bs/1')
            
            inputs = get(busselector,'InputSignals');
            expected = {'A429_BUS_1_MSG_1';'A429_BUS_1_MSG_2';'A429_BUS_1_MSG_3'};
            testCase.verifyEqual(inputs,expected)
            
        end
        
        function testSimpleA429ReceiveWithRefresh(testCase)
            
            % [ SETUP ]
            archivename = fullfile(testCase.filefolder_, 'A429_simple01.zip');
            unzip(archivename,pwd)
            
            modelname = 'readsimplea429';
            new_system(modelname)
            load_system(modelname)
            closeModel = onCleanup(@() bdclose(modelname));
            
            %
            % Add blocks in the model
            %
            configurationblockname = [modelname,'/Configure'];
            configurationblock = add_block('lib_ed247/ED247_Configuration', configurationblockname);
            
            receiveblockname = [modelname,'/Receive'];
            receiveblock = add_block('lib_ed247/ED247_Receive', receiveblockname);
                        
            %
            % Configure blocks
            %   - configuration file
            %   - Enable refresh
            %
            set(configurationblock, 'configurationFilename', '''a429_01_ecic_in.xml''')
            set(receiveblock, 'enable_refresh', 'on')
            
            % Run SIM to update diagram only (do not care about warnings)
            warning('off')
            sim(modelname,'StopTime','0');
            warning('on')
            
            receiveobj = ed247.blocks.Receive(receiveblock);
            receiveobj.fWaitbar = @(varargin) MockWaitbar.waitbar(varargin{:});
            
            % [ EXERCISE ]
            createBus(receiveobj)
            
            % [ VERIFY ]
            receivesubsysblock = find_system(modelname,'FindAll','on','SearchDepth',1,'Type','block','Name','Receive');
            testCase.assertLength(receivesubsysblock, 1, ...
                sprintf('Model should contain 1 block named %s', 'Receive'))
            testCase.assertEqual(get(receivesubsysblock,'BlockType'), 'SubSystem', ...
                sprintf('Block %s should be a subsystem', 'Receive'))
            
            ports = get(receivesubsysblock,'PortHandles');
            testCase.verifyEmpty(ports.Inport, 'Receive subsystem should not have any inport')
            testCase.verifyLength(ports.Outport, 1, 'Receive subsystem should have 1 outport')
            
            busselector = add_block('simulink/Signal Routing/Bus Selector',[modelname,'/bs']);
            pos = get(receivesubsysblock,'Position');
            set(busselector, 'Position', pos + [100,0,100,0])
            add_line(modelname,'Receive/1','bs/1')
            
            inputs = get(busselector,'InputSignals');
            inputs = vertcat(inputs{:});
            expected = {'A429_BUS_1_MSG_1';'A429_BUS_1_MSG_2';'A429_BUS_1_MSG_3'};
            testCase.verifyEqual(inputs(:,1),expected)
            
            content = inputs(:,2);
            content = [content{:}];
            expected = repmat({'data';'refresh'},1,3);
            testCase.verifyEqual(content,expected, 'Each signal should contain data and refresh fields')
            
        end
        
    end
        
    %% PRIVATE PROPERTIES
    properties (Access = private)
        filefolder_
    end
    
end