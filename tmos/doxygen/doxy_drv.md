# TMOS Driver System {#doxy_drv}

### DCR control codes 

 * DCR_OPEN
 
  When a client tries to open a handle, the kernel calls the driver's DCR with
  DCR_OPEN the driver must set hnd->res to RES_OK if the handle can be open
 
 * DCR_CANCEL
 
  This control is sent when the client wants to cancel the handle operation.
  Usually the client first checks for FLG_BUSY and only if it is set it calls
  cancel(). But there is a time between the check and the call in which an
  interrupt can complete the operation. So when the driver receives DCR_CANCEL
  the operation may or may not be completed.
 
  The driver should do nothing if there is no operation in progress (FLG_BUSY
  can be checked if necessarily). The driver can also ignore this control code
  if no cancellation mechanism is supported.
  When it is possible to cancel the operation, the driver must set status with
  FLG_SIGNALED. If there is some data transfered the _FLG_OK_ must also be set.

### DSR processing
 
  The driver DSR() function is called when a client requests some handle
  operation.
 
####  Handle operations
  Most common handle operations are simple read and write. These operations are
  signaled with FLG_READ and FLG_WRITE respectively in the hnd->cmd field.
  Some drivers may support simultaneous read and write (swap) operation.
  Another simple operation is a Command that can be requested with FLG_COMMAND.
  The command operation is a driver specific operation and not every driver can
  support it.
 
  In addition to the basic operations, the client can set FLG_LOCK. This makes
  the basic operation exclusive in some way. Typically, when a lock operation
  is requested, the driver must lock to the this client and will not start
  requests from other clients.
  In this way a client can perform a sequence of transactions without being
  interrupted from other clients. To end the sequence the client must request a
  normal operation (without FLG_LOCK).
 
  The lock flag may also have side effects. For example when a lock operations
  is requested from the SPI driver, it asserts the chip select (specified in
  the mode) and keeps it asserted for the whole sequence.
  Many SPI devices (flashes, LCDs etc) require a complete command sequence in
  single CS assertion. The command usually has a header and then the real data.
  If this has to be done in a single read/write the header and the data must be
  consequent in the memory which usually is not the case. However with a lock
  sequence there is no problem. First the header can be transferred and then
  the data with another request. The requests from the client with two memory
  blocks, but to the SPI device they appear a single transaction.
 
####  Status update
  In the first release of the driver system, the drivers had to set the status
  and signal the client only. This approach is OK for simple algorithms where
  the client makes a request and waits for a signal.
  The practice showed however that it is source for bugs and problems if the
  client by mistake places a second operation with the same handle while the
  driver is still working on the first request. Also sometimes the client can
  try to free the handle in the cleanup and has no record if there is pending
  operation with that handle. To solve such problems the drivers update few
  flags in hnd->res so the clients can have reliable information about the
  handle status.
 
 * FLG_BUSY
  The driver's *DSR()* must set FLG_BUSY if the handle processing will continue
  after the function return.
  While the handle is busy the client can only try to cancel, and it should not
  do anything else. In fact the client does not have to check this flag. It is
  checked internally in the CHandle methods.

  