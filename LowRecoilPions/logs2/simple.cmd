
# generated by jobsub_lite
# 
universe           = vanilla
executable         = simple.sh
arguments          = data_me1A_xrootd_plist.txt mc_me1A_xrootd_plist.txt

output             = runEventLoop_job.sh2023_02_21_1314402559902d-9ab6-415c-9840-73f9321691c4cluster.$(Cluster).$(Process).out
error              = runEventLoop_job.sh2023_02_21_1314402559902d-9ab6-415c-9840-73f9321691c4cluster.$(Cluster).$(Process).err
log                = runEventLoop_job.sh2023_02_21_1314402559902d-9ab6-415c-9840-73f9321691c4cluster.$(Cluster).$(Process).log


JOBSUBJOBSECTION=$(Process)


environment        = CLUSTER=$(Cluster);PROCESS=$(Process);JOBSUBJOBSECTION=$(JOBSUBJOBSECTION);CONDOR_TMP=/nashome/s/sultana/.cache/jobsub_lite/js_2023_02_21_131440_2559902d-9ab6-415c-9840-73f9321691c4;BEARER_TOKEN_FILE=.condor_creds/minerva.use;CONDOR_EXEC=/tmp;DAGMANJOBID=$(DAGManJobId);GRID_USER=sultana;JOBSUBJOBID=$(CLUSTER).$(PROCESS)@jobsub01.fnal.gov;EXPERIMENT=minerva;IFDH_CP_MAX_RETRIES=1
rank               = Mips / 2 + Memory
job_lease_duration = 3600
notification       = Never
transfer_output    = True
transfer_error     = True
transfer_executable= True
transfer_input_files = runEventLoop_job.sh,simple.cmd
transfer_output_files = simple.cmd,simple.sh,runEventLoop_job.sh
when_to_transfer_output = ON_EXIT_OR_EVICT

request_memory = 6144.0
request_disk = 5242880.0KB

+JobsubClientDN="/DC=org/DC=cilogon/C=US/O=Fermi National Accelerator Laboratory/OU=People/CN=Mehreen Sultana/CN=UID:sultana/CN=1956228813"
+JobsubClientIpAddress="131.225.67.85"
+JobsubServerVersion="lite_v1_0"
+JobsubClientVersion="lite_v1_0"
+JobsubClientKerberosPrincipal="sultana@FNAL.GOV"
+JOB_EXPECTED_MAX_LIFETIME = 7200.0
notify_user = sultana@fnal.gov

# set command to user executable for jobsub_q
+JobsubCmd = "runEventLoop_job.sh"


+AccountingGroup = "group_minerva.sultana"


+Jobsub_Group="minerva"
+JobsubJobId="$(CLUSTER).$(PROCESS)@jobsub01.fnal.gov"
+Drain = False


+DESIRED_SITES = "FermiGrid"


+GeneratedBy ="lite_v1_0 jobsub01.fnal.gov"


+DESIRED_usage_model="DEDICATED,OPPORTUNISTIC"

requirements  = target.machine =!= MachineAttrMachine1 && target.machine =!= MachineAttrMachine2 && (isUndefined(DesiredOS) || stringListsIntersect(toUpper(DesiredOS),IFOS_installed)) && (stringListsIntersect(toUpper(target.HAS_usage_model), toUpper(my.DESIRED_usage_model))) && ((isUndefined(target.GLIDEIN_Site) == FALSE) && (stringListIMember(target.GLIDEIN_Site,my.DESIRED_Sites)))



+SingularityImage="/cvmfs/singularity.opensciencegrid.org/fermilab/fnal-wn-sl7:latest"


#
# this is supposed to get us output even if jobs are held(?)
#
+SpoolOnEvict = false
#
#
#

# Credentials

use_oauth_services = minerva

#minerva_oauth_permissions_7c8847dada = " compute.read compute.create compute.cancel compute.modify storage.read:/minerva storage.create:/fermigrid/jobsub/jobs storage.read:/minerva/resilient/jobsub_stage storage.create:/minerva/scratch/users/sultana storage.create:/minerva/resilient/jobsub_stage storage.create:/minerva/persistent/users/sultana "




+x509userproxy = "/tmp/x509up_minerva_Analysis_49107"

delegate_job_GSI_credentials_lifetime = 0


queue 1