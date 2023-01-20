universe          = vanilla
executable        = /fife/local/scratch/uploads/minerva/sultana/2023-01-19_235202.946992_2088/runEventLoop_job.sh_20230119_235204_2141676_0_1_wrap.sh
arguments         =  data_me1A_xrootd_plist.txt  mc_me1A_xrootd_plist.txt 
output                = /fife/local/scratch/uploads/minerva/sultana/2023-01-19_235202.946992_2088/runEventLoop_job.sh_20230119_235204_2141676_0_1_cluster.$(Cluster).$(Process).out
error                 = /fife/local/scratch/uploads/minerva/sultana/2023-01-19_235202.946992_2088/runEventLoop_job.sh_20230119_235204_2141676_0_1_cluster.$(Cluster).$(Process).err
log                   = /fife/local/scratch/uploads/minerva/sultana/2023-01-19_235202.946992_2088/runEventLoop_job.sh_20230119_235204_2141676_0_1_.log
environment   = CLUSTER=$(Cluster);PROCESS=$(Process);CONDOR_TMP=/fife/local/scratch/uploads/minerva/sultana/2023-01-19_235202.946992_2088;CONDOR_EXEC=/tmp;DAGMANJOBID=$(DAGManJobId);IFDH_CP_MAX_RETRIES=1;GRID_USER=sultana;IFDH_BASE_URI=http://samweb.fnal.gov:8480/sam/minerva/api;JOBSUBJOBID=$(CLUSTER).$(PROCESS)@jobsub02.fnal.gov;EXPERIMENT=minerva
rank                  = Mips / 2 + Memory
job_lease_duration = 3600
notification = Never
when_to_transfer_output = ON_EXIT_OR_EVICT
transfer_output                 = True
transfer_output_files = .empty_file
transfer_error                  = True
transfer_executable         = True
transfer_input_files = /fife/local/scratch/uploads/minerva/sultana/2023-01-19_235202.946992_2088/runEventLoop_job.sh
+JobsubClientDN="/DC=org/DC=cilogon/C=US/O=Fermi National Accelerator Laboratory/OU=People/CN=Mehreen Sultana/CN=UID:sultana/CN=1927892720"
+JobsubClientIpAddress="131.225.67.69"
+Owner="sultana"
+JobsubServerVersion="1.3.5.1"
+JobsubClientVersion="1.3.5"
+JobsubClientKerberosPrincipal="sultana@FNAL.GOV"
+JobsubClientPython="/cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/python/v2_7_13d/Linux64bit+3.10-2.17/bin/python...2.7.13"
+JOB_EXPECTED_MAX_LIFETIME = 57600
+PNFS_INPUT_FILES = "/pnfs/minerva/resilient/tarballs/sultana_Jan192023_LowRecPion.tgz" 
notify_user = sultana@fnal.gov
x509userproxy = /var/lib/jobsub/creds/proxies/minerva/x509cc_sultana_Analysis
+AccountingGroup = "group_minerva.sultana"
+Jobsub_Group="minerva"
+JobsubJobId="$(CLUSTER).$(PROCESS)@jobsub02.fnal.gov"
+Drain = False
+GeneratedBy ="NO_UPS_VERSION jobsub02.fnal.gov"
+DESIRED_usage_model = "DEDICATED,OPPORTUNISTIC"
request_disk = 2GB
request_memory = 6GB
requirements  = target.machine =!= MachineAttrMachine1 && target.machine =!= MachineAttrMachine2  && (isUndefined(DesiredOS) || stringListsIntersect(toUpper(DesiredOS),IFOS_installed)) && (stringListsIntersect(toUpper(target.HAS_usage_model), toUpper(my.DESIRED_usage_model)))


queue 1