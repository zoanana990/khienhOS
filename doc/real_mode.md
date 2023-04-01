# Real Mode

## Disk

### Disk access and how it works
- File does not exist
  - File system are kernel implemented, they are not the responsibility of the hard disk
  - The files for hard disk is just a data sector, no other meanings
  - Implementing a filesystem requires the kernel programmer to create a filesystem driver 
    for the target filesystem
- Data is read and written in sector
  - Data is read and written in sectors **typically 512 byte blocks**
  - Reading the sector of a disk will return 512 bytes of data for the chosen sector
- LBA (Logical block address)
  - This is the modern way to read from a hard disk
  - LBA allows us to read from the disk if we are reading block from a very large file
  - LBA0 = first sector, LBA1 = second sector

### Reading from the hard disk