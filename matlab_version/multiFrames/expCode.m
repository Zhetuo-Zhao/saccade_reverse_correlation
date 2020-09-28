% Clear the workspace and the screen
sca;
close all;
clearvars;
rng('shuffle');


sessionIdx=4;



outputFolder=['./data/' date '/s' num2str(sessionIdx) '/'];
if ~exist(outputFolder, 'dir')
   mkdir(outputFolder)
end 

InitializePsychSound(1);
PsychDefaultSetup(2);
Screen('Preference','SkipSyncTests',1); 

screens = Screen('Screens');
screenNumber = 1;

white = WhiteIndex(screenNumber);
black = BlackIndex(screenNumber);
grey = white / 2;
inc = white - grey;

[window, windowRect] = PsychImaging('OpenWindow', screenNumber, grey,[0 0 1920 1080]);
[screenXpixels, screenYpixels] = Screen('WindowSize', window);
ifi = Screen('GetFlipInterval', window);

escapeKey = KbName('ESCAPE');
leftKey = KbName('LeftArrow');
rightKey = KbName('RightArrow');

[xCenter, yCenter] = RectCenter(windowRect);
Screen('BlendFunction', window, 'GL_SRC_ALPHA', 'GL_ONE_MINUS_SRC_ALPHA');
topPriorityLevel = MaxPriority(window);

stopSession=0;
for trialIdx=1:100
    DrawFormattedText(window, sprintf('trial %d',trialIdx), 50, 50, white);
    
    %% load image
    % mask = imread('C:\Users\zzhao\Box\reverseCorrelation\stim\noiseMask\checkerMask.bmp');
    % maskTexture = Screen('MakeTexture', window, mask);
    trial.SNR=0.2;
    trial.stimFolder=['C:\Users\zzhao\Desktop\stim\c' num2str(100*trial.SNR) '\' ];
    
    trial.frameNum=10; 
    trial.frameDur=3; % 3 frames for 60 Hz screen
    trial.frameTiming=zeros(1,trial.frameNum+1);
    trial.stimIdx=floor(rand(1,trial.frameNum)*600)+1;
    trial.ONflag=rand()>0.5;
    
    disp('loading stimuli');
    trial.mask=imread('C:\Users\zzhao\Desktop\stim\noiseMask\checkerMask_3.bmp');
    if trial.ONflag
        trial.pi=floor(rand()*8)+1;
        for fi=1:trial.frameNum
            stim{fi} = imread([trial.stimFolder 'rand' num2str(trial.stimIdx(fi)) '\' sprintf('stim_p%d_c%d_%d.bmp',trial.pi,100*trial.SNR,trial.stimIdx(fi))]);
        end
    else
        trial.pi=0;
        for fi=1:trial.frameNum
            stim{fi} = imread([trial.stimFolder 'rand' num2str(trial.stimIdx(fi)) '\' sprintf('noise_c%d_%d.bmp',100*trial.SNR,trial.stimIdx(fi))]);
        end
    end
    disp('loading completes');

    maskTexture = Screen('MakeTexture', window, trial.mask);
    for fi=1:trial.frameNum
        stimTexture{fi} = Screen('MakeTexture', window, stim{fi});
    end
    
    Priority(topPriorityLevel);
    Screen('DrawDots', window, [xCenter yCenter], 10, [1 0 0], [], 1);
    Screen('Flip', window);
    WaitSecs(rand()*2+1);
    Screen('FillRect', window, grey*ones(1,3));
    Screen('Flip', window);
    WaitSecs(0.2);
    Priority(topPriorityLevel);
    
    Screen('DrawTexture', window, stimTexture{1}, [], [], 0);
    vbl=Screen('Flip', window); 
    trial.frameTiming(1)=vbl;
    for fi=2:length(stimTexture)
        Screen('DrawTexture', window, stimTexture{fi}, [], [], 0);
        vbl=Screen('Flip', window, vbl+(3-0.5)*ifi);
        trial.frameTiming(fi)=vbl;
    end
    Screen('DrawTexture', window, maskTexture, [], [], 0);
    vbl=Screen('Flip', window, vbl+(3-0.5)*ifi);
    trial.frameTiming(trial.frameNum+1)=vbl;
    
    pahandle = PsychPortAudio('Open', [], 1, 1, 48E3, 2);
    PsychPortAudio('Volume', pahandle, 0.5);
    myBeep = MakeBeep(500, 0.5, 48E3);
    PsychPortAudio('FillBuffer', pahandle, [myBeep; myBeep]);
    PsychPortAudio('Start', pahandle, 1, 0, 1);

    waitResp = 1;
    while waitResp
        % Check the keyboard to see if a button has been pressed
        [keyIsDown,secs, keyCode] = KbCheck;
        if keyCode(leftKey)
            waitResp = 0;
            trial.resp=0;       
        elseif keyCode(rightKey)
            waitResp = 0;
            trial.resp=1;
            
        elseif keyCode(escapeKey)
            waitResp = 0;
            stopSession=1;
        end
    end

    if stopSession
        break;
    end
    fileName=['trial_' num2str(round(rand()*1E6),'%06.f') '_' num2str(trialIdx,'%03.f') '.mat'];
    save([outputFolder sprintf('trial%') fileName],'trial');
    
end
sca;
