#***SAMWeb Client Python API***

---

## **Administrative Functionality**

### listApplications
Return a list of applications<br/>

***Arguments:***
  1. SAMWeb Instance
  2. queryCriteria: A dictionary of key, value pairs

    def listApplications(samweb, **queryCriteria)

### addApplication
POST a new application to the SAMWeb instance<br/>

***Arguments:***
  1. SAMWeb Instance
  2. family: Application family
  3. name: Application name
  4. version: Application version

    def addApplication(samweb, family, name, version)

### listUsers
Return a list of all users registered to the SAMWeb instance<br/>

***Arguments:***
  1. SAMWeb Instance

    def listUsers(samweb)

### describeUser
Return details of user from argument 'username' in JSON format<br/>

***Arguments:***
  1. SAMWeb Instance
  2. username: SAM username of user registered in SAMWeb Instance

    def describeUser(samweb, username)

### describeUserText
Return details of users from argument 'username' in text format<br/>

***Arguments:***
  1. SAMWeb Instance
  2. username: SAM username of user registered in SAMWeb Instance

    def describeUserText(samweb, username)

### addUser
Add a new user to the SAMWeb Instance<br/>

***Arguments:***
  1. SAMWeb Instance
  2. username: SAM username of user to be registered in SAMWeb Instance
  3. firstname: First name of new user (Default: None)
  4. lastname: Last name of new user (Default: None)
  5. email: Email address of new user (Default: None)
  6. uid: Identification number of new user (Defaults: None)
  7. groups: Privilege groups the new user is to be registered into (Default: None)

    def addUser(samweb, username, firstname=None, lastname=None, email=None, uid=None, groups=None)

### modifyUser
Modify an existing SAMWeb user's details<br/>

***Arguments:***
  1. SAMWeb Instance
  2. username: SAM username of user to be modified
  3. **args: Dictionary of key, value pairs corresponding to user options to be modified

    def modifyUser(samweb, username, **args)

### getAvailableValues
Returns the names that may be used in calls to listValues() and addValues()<br/>

***Arguments:***
  1. SAMWeb Instance

    def getAvailableValues(samweb)

### listValues
List values contained in the database<br/>

***Arguments:***
  1. SAMWeb Instance
  2. vtype: String with values to return (i.e. data_tiers, groups)

    def listValues(samweb, vtype)

### addValue
Add a new value to the database<br/>

***Arguments:***
  1. SAMWeb Instance
  2. vtype: String giving the type of value to add (i.e. data_tiers, groups)
  3. *args: Arguments to be passed to server
  4. **kwargs: Keyword arguments to be passed to server

    def addValue(samweb, vtype, *args, **kwargs)

### listParameters
Returns a list of the defined parameters<br/>

***Arguments:***
  1. SAMWeb Instance

    def listParameters(samweb)

### listParameterValues
Lists all the values for a particular parameter<br/>

***Arguments:***
  1. SAMWeb Instance
  2. param: Name of the parameter having its values queried

    def listParameterValues(samweb, param)

### addParameter
Add a new parameter to the database<br/>

***Arguments:***
  1. SAMWeb Instance
  2. name: String that contains name of the new parameter
  3. data_type: Data type of new parameter
  4. category: Category of the new parameter (Default: None)

    def addParameter(samweb, name, data_type, category=None)

### listDataDisks
Returns a list of defined data disks<br/>

***Arguments:***
  1. SAMWeb Instance

    def listDataDisks(samweb)

### addDataDisk
Add a new data disk to the database<br/>

***Arguments:***
  1. SAMWeb Instance
  2. mount_point: The path mount point for the new disk

    def addDataDisk(samweb, mount_point)
---

## **Definition Functionality**

### listDefinitions
List definitions matching given query parameters<br/>

***Arguments:***
  1. SAMWeb Instance
  2. stream: Boolean indicating if results should be returned progressively
  3. **queryCriteria: Dictionary of key, value pairs containing the terms of the definition query

    def listDefinitions(samweb, stream=False, **queryCriteria)

### descDefinitionDict
Returns a dictionary with a description of a dataset definition<br/>

***Arguments:***
  1. SAMWeb Instance
  2. defname: Name of the definition to be described

    def descDefinitionDict(samweb, defname)

### descDefinition
Returns a textual description of a dataset definition<br/>

***Arguments:***
  1. SAMWeb Instance
  2. defname: Name of the definition to be described

    def descDefinition(samweb, defname)

### createDefinition
Create a new dataset definition<br/>

***Arguments:***
  1. SAMWeb Instance
  2. defname: Name to be assigned to the new dataset definition
  3. dims: String containg the dimensions of the new definition
  4. user: Username to be attached to new definition (Default: None)
  5. group: Group name for the new definition to be assigned to (Default: None)
  6. description: Description of the new definition (Default: None)

    def createDefinition(samweb, defname, dims, user=None, group=None, description=None)

### modifyDefinition
Modify a dataset definition<br/>

***Arguments:***
  1. SAMWeb Instance
  2. existing_defname: Name of the definition to be modified
  3. defname: String containing new name for the modified definition (Default: None)
  4. description: String containg new description for the definition (Default: None)

    def modifyDefinition(samweb, existing_defname, defname=None, description=None)

### deleteDefinition
Delete a dataset definition from the database<br/>

***Arguments:***
  1. SAMWeb Instance
  2. defname: Name of the definition to be deleted

    def deleteDefinition(samweb, defname)

### takeSnapshot
Create a snapshot of an existing definition<br/>

***Arguments:***
  1. SAMWeb Instance
  2. defname: Name of the definition to be snapshotted
  3. group: Group for the snapshot (Internally defaults to experiment name if none provided)

    def takeSnapshot(samweb, defname, group=None)
---

## **File Functionality**

### getAvailableDimensions
Return a list of the available dimensions<br/>

***Arguments:***
  1. SAMWeb Instance

    def getAvailableDimensions(samweb)

### listFiles
List files matching either a dataset definition or a dimensions string<br/>

***Arguments:***
  1. SAMWeb Instance
  2. dimensions: String containing the dimension to have its files listed (Default: None)
  3. defname: String containing definition to have its files listed (Default: None)
  4. fileinfo: Boolean where True indicates file_id, file_size and event_count should be returned
  5. stream: Boolean with True indicating that files should be delivered progressively

    def listFiles(samweb, dimensions=None, defname=None, fileinfo=False, stream=False)

### listFilesSummary
Returns a summary of files matching either a dataset definition or a dimensions string <br/>

***Arguments:***
  1. SAMWeb Instance
  2. dimensions: String designating the dimension to be summarized (Default: None)
  3. defname: String containg the name of the definition to be summarized (Default: None)

    def listFilesSummary(samweb, dimensions=None, defname=None)

### expandDims
Expand the given dimensions<br/>

***Arguments:***
  1. SAMWeb Instance
  2. dimensions: Dimension to be expanded

    def expandDims(samweb, dimensions)

### parseDims
Parse through the given dimensions for debugging purposes<br/>

***Arguments:***
  1. SAMWeb Instance
  2. dimensions: A dimensions string
  3. mode: A string that selects the behavior from 'count', 'summary', 'fileinfo' or 'list'

    def parseDims(samweb, dimensions, mode=False)

### countFiles
Return a count of files matching either a dataset definition or a dimensisons string<br/>

***Arguments:***
  1. SAMWeb Instance
  2. dimensions: Dimensions to have files counted (Default: None)
  3. defname: Defintion to have files counted (Default: None)

    def countFiles(samweb, dimensions=None, defname=None)

### listFilesAndLocations
List files and their locations based on location path, dimensions, or defname arguments <br/>

***Arguments:***
  1. SAMWeb Instance
  2. filter_path: String or list of strings (Default: None)
  3: dimensions: String containg dimensions to be returned (Default: None)
  4. defname: String containing name of definition to be returned (Default: None)

    def listFilesAndLocations(samweb, filter_path=None, dimensions=None, defname=None,
      checksums=False, schema=None, structured_output=True)

### locateFile
Return locations for this file<br/>

***Arguments:***
  1. SAMWeb Instance
  2. filenameorid: Name or id number of file to be located

    def locateFile(samweb, filenameorid)

### locateFiles
Returns the locations of multiple files in the form { filenameorid : location }<br/>

***Arguments:***
  1. SAMWeb Instance
  2. filenameorids: List of file names or id numbers to be located

    def locateFiles(samweb, filenameorids)

### locateFilesIterator
Given an iterable of file names or ids, return an iterable object with (name or id, locations) pairs. Convienient wrapper for locateFiles<br/>

***Arguments:***
  1. SAMWeb Instance
  2. iterable: Iterable object of file names or identification numbers
  3. chunksize: The number of files to be queried on each pass

    def locateFilesIterator(samweb, iterable, chunksize=50)

### addFileLocation
Add a SAM location for a file<br/>

***Arguments:***
  1. SAMWeb Instance
  2. filenameorid: Name or id number of the file to be added
  3. location: Location to be added for the file specified by filenameorid

    def addFileLocation(samweb, filenameorid, location)

### removeFileLocation
Remove a SAM location for a file<br/>

***Arguments:***
  1. SAMWeb Instance
  2. filenameorid: Filename or id number of the file to have its location removed
  3. location: SAM location to be removed for this file

    def removeFileLocation(samweb, filenameorid, location)

### getFileAccessUrls
Return all the URL's by which the specified file may be accessed<br/>

***Arguments:***
  1. SAMWeb Instance
  2. filenameorid: Filename or id number of the file
  3. schema: A comma seperated list of acceptable protocols (ex. gsiftp, xrootd, ...)
  4. locationfilter: Only return URL's for files that are contained in the filtered location (Default: None)

    def getFileAccessUrls(samweb, filenameorid, schema, locationfilter=None)

### getMetadata
Return metadata as a dictionary<br/>

***Arguments:***
  1. SAMWeb Instance
  2. filenameorid: Filename or id number of the file having its metadata queried
  3. locations: Boolean with True value indicating that the location of this file should also be returned (Default: None)
  4. basic: Boolean with True indicating that only basic metadata should be returned (Default: False)

    def getMetadata(samweb, filenameorid, locations=False, basic=False)

### getMultipleMetadata
Return a list of metadata dictionaries. Returns no result for non-existent files<br/>

***Arguments:***
  1. SAMWeb Instance
  2. filenameorids: Filenames or id numbers of files having their metadata queried
  3. locations: Boolean with True indicating that location data should be included (Default: None)
  4. asJSON: Boolean with True indicating that the unencoded JSON string should be returned (Default: False)
  5: basic: Boolean with True indicating that only basic metadata should be returned (Default: None)

    def getMultipleMetadata(samweb, filenameorids, locations=False, asJSON=False, basic=False)

### getMetadataIterator
Given an iterable of file names or id numbers, return an iterable object with their metadata. Convenience wrapper around getMultipleMetadata.<br/>

***Arguments:***
  1. SAMWeb Instance
  2. iterable: Iterable object of filenames or id numbers.
  3. locations: Boolean with True indicating that location data should be included (Default: None)
  4. basic: Boolean with True indicating that only basic metadata should be returned (Default: None)
  5. chunksize: The number of files to query on each pass (Default: 50)

    def getMetadataIterator(samweb, iterable, locations=False, basic=False, chunksize=50)

### getMetadataText
Return metadata for a file as a string<br/>

***Arguments:***
  1. SAMWeb Instance
  2. filenameorid: Filename or id number of the file having its metadata queried
  3. format: A format string to have return output mapped into (Default: None)
  4. locations: Boolean with True indicating that location data should be included (Default: None)
  5. basic: Boolean with True indicating that only basic metadata should be returned (Default: None)

    def getMetadataText(samweb, filenameorid, format=None, locations=False, basic=False)

### getFileLineage
Return lineage information for a file<br/>

***Arguments:***
  1. SAMWeb Instance
  2. lineagetype: Options are 'parents' or 'children'
  3. filenameorid: Filename or id number of the file having its metadata queried

    def getFileLineage(samweb, lineagetype, filenameorid)

### validateFileMetadata
Check the validity of the metadata<br/>

***Arguments:***
  1. SAMWeb Instance
  2. md: Dictionary containing the metadata to be validated (Default: None)
  3. mdfile: File object containing metadata (Default: None)

    def validateFileMetadata(samweb, md=None, mdfile=None)

### declareFile
Declare a new file to this SAMWeb instance<br/>

***Arguments:***
  1. SAMWeb Instance
  2. md: Dictionary containing the metadata to be validated (Default: None)
  3. mdfile: File object containing metadata (Default: None)

    def declareFile(samweb, md=None, mdfile=None)

### modifyFileMetadata
Modify the file metadata for a given file<br/>

***Arguments:***
  1. SAMWeb Instance
  2. filename: Name of the file having its file metadata modified
  3. md: Dictionary containing the metadata to be validated (Default: None)
  4. mdfile: File object containing metadata (Default: None)

    def modifyFileMetadata(samweb, filename, md=None, mdfile=None)

### modifyMetadata
Modify metadata for one or more files. The metadata must be in the form of a list of metadata objects containing the file_name and the fields to change the arguments<br/>

***Arguments:***
  1. SAMWeb Instance
  2. md: Dictionary containing the metadata to be validated (Default: None)
  3. mdfile: File object containing metadata (Default: None)
  4. continue_on_error: Boolean with True indicating an error in a files metadata will not prevent the other files metadata from being processed (Default: False)

    def modifyMetadata(samweb, md=None, mdfile=None, continue_on_error=False)

### retireFile
Retire a file<br/>

***Arguments:***
  1. SAMWeb Instance
  2. filename: Name of the file to be retired

    def retireFile(samweb, filename)

### verifyFileChecksum
Verify checksum of local file against what is stored in SAM<br/>

***Arguments:***
  1. SAMWeb Instance
  2. path: Either the filename or the path to the physical file. The path must be given if checksum is None
  3. checksum: A list of checksums to verify against. If given the file will not be opened (Default: None)
  4. algorithms: A list of preferred algorithms in order (Default: None)

    def verifyFileChecksum(samweb, path, checksum=None, algorithms=None)
---

## **Project Functionality**

### listProjects
List projects matching query parameters<br/>

***Arguments:***
  1. SAMWeb Instance
  2. stream: Boolean with True indicating that results should be delivered progressively
  3. queryCriteria: Dictionary containing arguments for project query

    def listProjects(samweb, stream=False, **queryCriteria)

### makeProjectName
Make a suitable project name from the provided string<br/>

***Arguments:***
  1. SAMWeb Instance
  2. description: Project description to be using in making a project name

    def makeProjectName(samweb, description)

### startProject
Start a SAM project on the station. Either defname or snapshot_id must be given<br/>

***Arguments:***
  1. SAMWeb Instance
  2. project: Project name
  3. defname: Definition name (Default: None)
  4. station: Station name (Defaults to experiment name) (Default: None)
  5. group: Group name (Defaults to experiment name)  (Default: None)
  6. user: User name (Defaults to user name from certificate) (Default: None)
  7. snapshot_id: ID number of snapshot (Default: None)

    def startProject(samweb, project, defname=None, station=None, group=None, user=None, snapshot_id=None)

### findProject
Return details about a specified project<br/>

***Arguments:***
  1. SAMWeb Instance
  2. project: The name of the project to be returned
  3. station: The station to be queried for the project (Default: None)

    def findProject(samweb, project, station=None)

### startProcess
Start a new process<br/>

***Arguments:***
  1. SAMWeb Instance
  2. projecturl: The URL of the SAM Project to be started
  3. appfamily: Application family
  4. appname: Application name
  5. appversion: Application version
  6. deliveryLocation: Location that files will be delivered (Default: None)
  7. node: Name of node that process will be run on (Default: None)
  8. user: Name of user (Default: User that owns the certificate being used for authentication)
  9. maxFiles: Maximum number of files to be delivered to the process (Default: None)
  10. description: String containing description of the process (Default: None)
  11. schemas: Comma seperated list of acceptable protocols (ex. gsiftp, xrootd, ...) (Default: None)

    def startProcess(samweb, projecturl, appfamily, appname, appversion, deliveryLocation=None,
        node=None,user=None, maxFiles=None, description=None, schemas=None)

### makeProcessUrl
Make the process URL from a project URL and process ID<br/>

***Arguments:***
  1. SAMWeb Instance
  2. projecturl: Project URL
  3. processid: Process ID

    def makeProcessUrl(samweb, projecturl, processid)

### getNextFile
Get the next file from the project<br/>

***Arguments:***
  1. SAMWeb Instance
  2. processurl: The process URL
  3. timeout: Timeout after obtaining no files in number of seconds. -1 to disable, 0 to return immediately. (Default: None)

    def getNextFile(samweb, processurl, timeout=None)

### releaseFile
Release a file that has been processed<br/>

***Arguments:***
  1. SAMWeb Instance
  2. processurl: URL for the process that has completed use of the file
  3. filename: Name of the file that processing has been completed on
  4. status: If status is 'ok', the file was successfully processed (Default: 'ok')

    def releaseFile(samweb, processurl, filename, status="ok")

### setProcessFileStatus
Sets the status of a file to the value of status<br/>

***Arguments:***
  1. SAMWeb Instance
  2. processurl: URL for the process the file belongs to
  3. filename: Name of the file that is having its status updated
  4. status: Value containing the new status for the file

    def setProcessFileStatus(samweb, processurl, filename, status="consumed")

### stopProcess
End an existing process<br/>

***Arguments:***
  1. SAMWeb Instance
  2. processurl: The URL of the process that is being ended

    def stopProcess(samweb, processurl)

### stopProject
Ends a project<br/>

***Arguments:***
  1. SAMWeb Instance
  2. projecturl: URL of the project to be stopped

    def stopProject(samweb, projecturl)

### projectSummary
Returns a summary of the project in JSON format<br/>

***Arguments:***
  1. SAMWeb Instance
  2. projecturl: URL of the project to be summarized

    def projectSummary(samweb, projecturl)

### projectSummaryText
Returns a summary of the project in text format<br/>

***Arguments:***
  1. SAMWeb Instance
  2. projecturl: URL of the project to be summarized

    def projectSummaryText(samweb, projecturl)

### projectRecoveryDimension
Get the dimensions to create a recovery dataset<br/>

***Arguments:***
  1. SAMWeb Instance
  2. projectnameorurl: Project name or URL
  3. useFileStatus: Use the status of the last file seen by a process (Default: None)
  4. useProcessStatus: Use the status of the process (Default: None)

    def projectRecoveryDimension(samweb, projectnameorurl, useFileStatus=None, useProcessStatus=None)

### setProcessStatus
Mark the final status of a process<br/>

***Arguments:***
  1. SAMWeb Instance
  2. status: Must be either 'completed' or 'bad'
  3. projectnameorurl: Project name or URL
  4. processid: Process identifier (Default: None)
  5. process_desc: Description of the process (Default: None)

    def setProcessStatus(samweb, status, projectnameorurl, processid=None, process_desc=None)

### runProject
Run a SAM project<br/>

***Arguments:***
  1. SAMWeb Instance
  2. projectname: The name for the project (Default: None)
  3. defname: The definition name to use (Default: None)
  4. snapshot_id: The ID number of the snapshot to use (Default: None)
  5. callback: A single argument function invoked on each file (Default: None)
  6. deliveryLocation: Location that files will be delivered to (Default: None)
  7. node: Where the job will run (Default: None)
  8. station: Station that the project will be served files from (Default: None)
  9. maxFiles: Maximum number of files to be used in the project (Default: 0)
  10. schemas: Comma seperated list of transfer protocols to be used (ex. gsiftp, xrootd, ...)
  11. application: A three element sequence of (family, name, version) (Default: ('runproject','runproject',get_version()))
  12. nparallel: The number of processes to be run in parallel (Default: 1)
  13. quiet: Boolean with True indicating normal output should be suppressed (Default: False)

    def runProject(samweb, projectname=None, defname=None, snapshot_id=None, callback=None,
        deliveryLocation=None, node=None, station=None, maxFiles=0, schemas=None,
        application=('runproject','runproject',get_version()), nparallel=1, quiet=False )

### prestageDataset
Prestage the given dataset: Prestaging is not complete until the command exits, so it must continue to run until then<br/>

***Arguments:***
  1. SAMWeb Instance
  2. projectname: Name of the project (Default: None)
  3. defname: Name of the definition (Default: None)
  4. snapshot_id: ID number of the snapshot to be used (Default: None)
  5. maxFiles: Maximum number of files to be prestaged (Default: 0)
  6. station: Station that the files should be staged for (Default: None)
  7. deliveryLocation: Location that files will be delivered to (Default: None)
  8. node: Node that the data will be staged on (Default: None)
  9. nparallel: Number of processes to be run in parallel (Default: 1)

    def prestageDataset(samweb, projectname=None, defname=None, snapshot_id=None, maxFiles=0,
        station=None, deliveryLocation=None, node=None, nparallel=1)
---

## **Miscellaneous Functionality**

### serverInfo
Returns information about the server<br/>
***Arguments:***
  1. SAMWeb Instance
  2. authenticate
     - False: Default
     - True: Force an SSL connection


    def serverInfo(samweb, authenticate=False)
---
