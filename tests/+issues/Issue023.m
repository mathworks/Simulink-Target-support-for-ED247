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
        
        function testCreateBusSimpleA429ReceiveWithoutRefresh(testCase)
            
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
            %   - Disable refresh
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
        
        function testCreateBusSimpleA429ReceiveWithRefresh(testCase)
            
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
        
        function testCreateBusFullELACReceiveWithoutRefresh(testCase)
            
            % [ SETUP ]
            % Load expected signals table
            expecteddatafile = fullfile(testCase.filefolder_, 'ELACe2C_ECIC.mat');
            expecteddata = load(expecteddatafile);
            expecteddata = expecteddata.Receive;
            
            archivename = fullfile(testCase.filefolder_, 'ELAC_full.zip');
            unzip(archivename,pwd)
            
            modelname = 'readfullelac';
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
            %   - Disable refresh
            %
            set(configurationblock, 'configurationFilename', '''ELACe2C_ECIC.xml''')
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
            expected = convertStringsToChars(expecteddata.SignalName);
            testCase.verifyEqual(inputs,expected)
            
        end
        
        function testCreateBusFullELACReceiveWithRefresh(testCase)
            
            % [ SETUP ]
            % Load expected signals table
            expecteddatafile = fullfile(testCase.filefolder_, 'ELACe2C_ECIC.mat');
            expecteddata = load(expecteddatafile);
            expecteddata = expecteddata.Receive;
            
            archivename = fullfile(testCase.filefolder_, 'ELAC_full.zip');
            unzip(archivename,pwd)
            
            modelname = 'readfullelac';
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
            %   - Configuration file
            %   - Enable refresh
            %
            set(configurationblock, 'configurationFilename', '''ELACe2C_ECIC.xml''')
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
            mask = cellfun(@length,inputs) == 2;
            tmp = vertcat(inputs{mask});            
            signames = cell(size(inputs));
            signames(~mask) = inputs(~mask);
            signames(mask) = tmp(:,1);
            
            expected = convertStringsToChars(expecteddata.SignalName);
            testCase.verifyEqual(signames,expected)
            
            content = tmp(:,2);
            content = [content{:}];
            expected = repmat({'data';'refresh'},1,461);
            testCase.verifyEqual(content,expected, 'Each signal should contain data and refresh fields')
            
        end
        
        function testConnectLineGainSimpleA429Send(testCase)
            
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
            
            sendblockname = [modelname,'/Send'];
            sendblock = add_block('lib_ed247/ED247_Send', sendblockname);
                        
            %
            % Configure blocks
            %   - configuration file
            %   - Disable refresh
            %
            set(configurationblock, 'configurationFilename', '''a429_01_ecic_in.xml''')
            set(sendblock, 'enable_refresh', 'off')
            
            siggenblock = add_block('simulink/Math Operations/Gain', [modelname,'/testGain']);
            
            % Run SIM to update diagram only (do not care about warnings)
            warning('off')
            sim(modelname,'StopTime','0');
            warning('on')
            
            idxSrc = 1;
            idxDst = 1;
            
            sendobj = ed247.blocks.Send(sendblock);
            mockListDlg = MockListDlg(idxDst);
            sendobj.fListDlg = @(varargin) mockListDlg.listdlg(varargin{:});
            
            % [ EXERCISE ]
            connectLine(sendobj, siggenblock)
            
            % [ VERIFY ]
            srcline = get(siggenblock,'LineHandles');
            srcline = srcline.Outport(idxSrc);
            
            dstline = get(sendblock,'LineHandles');
            dstline = dstline.Inport(idxDst);
            testCase.verifyEqual(srcline,dstline, 'Source and destination lines should match')
            
        end
        
        function testConnectLineSubsystemSimpleA429Send(testCase)
            
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
            
            sendblockname = [modelname,'/Send'];
            sendblock = add_block('lib_ed247/ED247_Send', sendblockname);
                        
            %
            % Configure blocks
            %   - configuration file
            %   - Disable refresh
            %
            set(configurationblock, 'configurationFilename', '''a429_01_ecic_in.xml''')
            set(sendblock, 'enable_refresh', 'off', 'Position', [505,195,690,275])
            
            nports = 10;
            siggenblock = add_block('simulink/Ports & Subsystems/Subsystem', [modelname,'/testSub']);            
            Simulink.SubSystem.deleteContents(siggenblock)
            arrayfun(@(x) add_block('simulink/Ports & Subsystems/Out1', sprintf('%s/testSub/sig%03d', modelname, x)), 1:nports)
            set(siggenblock, 'Position', [140,200,255, 300 + 12*nports])
            
            % Run SIM to update diagram only (do not care about warnings)
            warning('off')
            sim(modelname,'StopTime','0');
            warning('on')
            
            idxSrc = [4,8];
            idxDst = [1,2];
            
            sendobj = ed247.blocks.Send(sendblock);
            mockListDlg = MockListDlg(idxSrc,idxDst);
            sendobj.fListDlg = @(varargin) mockListDlg.listdlg(varargin{:});
            
            % [ EXERCISE ]
            connectLine(sendobj, siggenblock)
            
            % [ VERIFY ]
            srcline = get(siggenblock,'LineHandles');
            srcline = srcline.Outport(idxSrc);
            
            dstline = get(sendblock,'LineHandles');
            dstline = dstline.Inport(idxDst);
            testCase.verifyEqual(srcline,dstline, 'Source and destination lines should match')
            
        end
        
        function testConnectLineSubsystemSimpleA429Send2Times(testCase)
            
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
            
            sendblockname = [modelname,'/Send'];
            sendblock = add_block('lib_ed247/ED247_Send', sendblockname);
                        
            %
            % Configure blocks
            %   - configuration file
            %   - Disable refresh
            %
            set(configurationblock, 'configurationFilename', '''a429_01_ecic_in.xml''')
            set(sendblock, 'enable_refresh', 'off', 'Position', [505,195,690,275])
            
            nports = 10;
            siggenblock = add_block('simulink/Ports & Subsystems/Subsystem', [modelname,'/testSub']);            
            Simulink.SubSystem.deleteContents(siggenblock)
            arrayfun(@(x) add_block('simulink/Ports & Subsystems/Out1', sprintf('%s/testSub/sig%03d', modelname, x)), 1:nports)
            set(siggenblock, 'Position', [140,200,255, 300 + 12*nports])
            
            % Run SIM to update diagram only (do not care about warnings)
            warning('off')
            sim(modelname,'StopTime','0');
            warning('on')
            
            idxSrc = [4,8];
            idxDst = [1,2];
                        
            % [ EXERCISE ]
            sendobj = ed247.blocks.Send(sendblock);
            mockListDlg = MockListDlg(idxSrc(2),idxDst(2));
            sendobj.fListDlg = @(varargin) mockListDlg.listdlg(varargin{:});
            connectLine(sendobj, siggenblock)
            
            sendobj = ed247.blocks.Send(sendblock);
            mockListDlg = MockListDlg(idxSrc(1),idxDst(1));
            sendobj.fListDlg = @(varargin) mockListDlg.listdlg(varargin{:});
            connectLine(sendobj, siggenblock)
            
            % [ VERIFY ]
            srcline = get(siggenblock,'LineHandles');
            srcline = srcline.Outport(idxSrc);
            
            dstline = get(sendblock,'LineHandles');
            dstline = dstline.Inport(idxDst);
            testCase.verifyEqual(srcline,dstline, 'Source and destination lines should match')
            
        end
        
        function testConnectLineGainFullELACSend(testCase)
            
            % [ SETUP ]
            archivename = fullfile(testCase.filefolder_, 'ELAC_full.zip');
            unzip(archivename,pwd)
            
            modelname = 'readfullelac';
            new_system(modelname)
            load_system(modelname)
            closeModel = onCleanup(@() bdclose(modelname));
            
            %
            % Add blocks in the model
            %
            configurationblockname = [modelname,'/Configure'];
            configurationblock = add_block('lib_ed247/ED247_Configuration', configurationblockname);
            
            sendblockname = [modelname,'/Send'];
            sendblock = add_block('lib_ed247/ED247_Send', sendblockname);
                        
            %
            % Configure blocks
            %   - configuration file
            %   - Disable refresh
            %
            set(configurationblock, 'configurationFilename', '''ELACe2C_ECIC.xml''')
            set(sendblock, 'enable_refresh', 'off')
            
            siggenblock = add_block('simulink/Math Operations/Gain', [modelname,'/testGain']);
            
            % Run SIM to update diagram only (do not care about warnings)
            warning('off')
            sim(modelname,'StopTime','0');
            warning('on')
            
            idxSrc = 1;
            idxDst = 150;
            
            sendobj = ed247.blocks.Send(sendblock);
            mockListDlg = MockListDlg(idxDst);
            sendobj.fListDlg = @(varargin) mockListDlg.listdlg(varargin{:});
            
            % [ EXERCISE ]
            connectLine(sendobj, siggenblock)
            
            % [ VERIFY ]
            srcline = get(siggenblock,'LineHandles');
            srcline = srcline.Outport(idxSrc);
            
            dstline = get(sendblock,'LineHandles');
            dstline = dstline.Inport(idxDst);
            testCase.verifyEqual(srcline,dstline, 'Source and destination lines should match')
            
        end
        
        function testConnectLineSubsystemFullELACSend(testCase)
            
            % [ SETUP ]
            archivename = fullfile(testCase.filefolder_, 'ELAC_full.zip');
            unzip(archivename,pwd)
            
            modelname = 'readfullelac';
            new_system(modelname)
            load_system(modelname)
            closeModel = onCleanup(@() bdclose(modelname));
            
            %
            % Add blocks in the model
            %
            configurationblockname = [modelname,'/Configure'];
            configurationblock = add_block('lib_ed247/ED247_Configuration', configurationblockname);
            
            sendblockname = [modelname,'/Send'];
            sendblock = add_block('lib_ed247/ED247_Send', sendblockname);
                        
            %
            % Configure blocks
            %   - configuration file
            %   - Disable refresh
            %
            set(configurationblock, 'configurationFilename', '''ELACe2C_ECIC.xml''')
            set(sendblock, 'enable_refresh', 'off', 'Position', [505,195,690,275])
            
            nports = 100;
            siggenblock = add_block('simulink/Ports & Subsystems/Subsystem', [modelname,'/testSub']);            
            Simulink.SubSystem.deleteContents(siggenblock)
            arrayfun(@(x) add_block('simulink/Ports & Subsystems/Out1', sprintf('%s/testSub/sig%03d', modelname, x)), 1:nports)
            set(siggenblock, 'Position', [140,200,255, 300 + 12*nports])
            
            % Run SIM to update diagram only (do not care about warnings)
            warning('off')
            sim(modelname,'StopTime','0');
            warning('on')
            
            idxSrc = [5,42,84];
            idxDst = [250,326,444];
            
            sendobj = ed247.blocks.Send(sendblock);
            mockListDlg = MockListDlg(idxSrc,idxDst);
            sendobj.fListDlg = @(varargin) mockListDlg.listdlg(varargin{:});
            
            % [ EXERCISE ]
            connectLine(sendobj, siggenblock)
            
            % [ VERIFY ]
            srcline = get(siggenblock,'LineHandles');
            srcline = srcline.Outport(idxSrc);
            
            dstline = get(sendblock,'LineHandles');
            dstline = dstline.Inport(idxDst);
            testCase.verifyEqual(srcline,dstline, 'Source and destination lines should match')
            
        end
        
    end
        
    %% PRIVATE PROPERTIES
    properties (Access = private)
        filefolder_
    end
    
end