direct='.\data\';
Dates={'22-Jun-2020'};
sessionNum=[4];
load('..\noiseEnergy.mat')

totalTrials=[];
for Di=1:length(Dates)
    for Si=1:sessionNum(Di)
        fileNames = dir([direct Dates{Di} '\s' num2str(Si) '\*.mat']);

        Trials=[]; 
        for i=1:length(fileNames)
            load([fileNames(i).folder '\' fileNames(i).name]);
            trial.EM=zeros(46,21);
            for fi=1:10
                trial.EMV{fi}=EM{trial.stimIdx(fi)};
                trial.EM=trial.EM+EM{trial.stimIdx(fi)};
            end
            trial.EM=trial.EM/10;
            Trials=[Trials trial];
        end
        save([direct Dates{Di} '\' sprintf('trials_s%d.mat',Si)], 'Trials');
        
        totalTrials=[totalTrials Trials];
        %% performance
        hit=0; miss=0; fr=0; cr=0;
        for i=1:length(Trials)

            if Trials(i).ONflag
                if Trials(i).resp
                    hit=hit+1;
                else
                    miss=miss+1;
                end
            else
                if Trials(i).resp
                    fr=fr+1;
                else
                    cr=cr+1;
                end
            end
        end

        disp(sprintf('S%d: correct=%.3f, hit=%.3f, false alarm=%.3f',Si,(hit+cr)/(hit+miss+fr+cr), hit/(hit+miss), fr/(fr+cr)))
    end
end

%% raw RTA
EM1=zeros(46,21); EM0=zeros(46,21);
EM_hit=zeros(46,21);
for i=1:length(totalTrials)
    if totalTrials(i).resp
        EM1=EM1+totalTrials(i).EM;
    else
        EM0=EM0+totalTrials(i).EM;
    end
end
EM2=EM0+EM1; EM2=EM2/sum(EM2(:)); 
EM1=EM1/sum(EM1(:)); 

figure('position',[100 100 800 360]); 
subplot(1,2,1); hold on; 
plot(freqs,mean(EM2,2)); 
plot(freqs,mean(EM1,2)); 
xlim([1 4]) 
legend({'all trial','resp 1'});
xlabel('spatial frequency (cpd)');
set(gca,'FontSize',14);

subplot(1,2,2); hold on; 
plot(orients/pi*180-45,mean(EM2,1)); 
plot(orients/pi*180-45,mean(EM1,1)); 
legend({'all trial','resp 1'});
xlabel('orientation (deg)');
set(gca,'FontSize',14);

figure('position',[100 100 800 360]); 
subplot(1,2,1); plot(freqs,mean(EM1./EM2,2));
xlim([1 4]) 
xlabel('spatial frequency (cpd)');
set(gca,'FontSize',14);

subplot(1,2,2); plot(orients/pi*180-45,mean(EM1./EM2,1)); 
xlabel('orientation (deg)');
set(gca,'FontSize',14);