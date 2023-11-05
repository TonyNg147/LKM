#define REG_SIZE (1)      //The size of the register is One byte
#define NUM_CTRL_REGS (1) // The number of the control register
#define NUM_STS_REGS  (5) // The number of the status register
#define NUM_DATA_REGS  (255) // The number of the data register
// Total Number of register
#define NUM_DEVICE_REGS  (NUM_CTRL_REGS + NUM_STS_REGS + NUM_DATA_REGS) 


/****************************STATUS_REGISTER_DEFINITION_START*********************************/ 
// The status register definition
// Pair Register [READ_COUNT_H_REG,READ_COUNT_L_REG]
// - Init value: 0x0001
// -  whenever read successfully, increase the read count number to 1

#define READ_COUNT_H_REG (0)
#define READ_COUNT_L_REG (1)

// The status register definition
// Pair Register [WRITE_COUNT_H_REG,WRITE_COUNT_L_REG]
// - Init value: 0x0000
// -  whenever write successfully, increase the read count number to 1

#define WRITE_COUNT_H_REG (2)
#define WRITE_COUNT_L_REG (3)

// - The init value 0x03
// - PAYLOAD:
//   bit0
//          0: The data register is not ready for reading
//          1: The data register is ready for reading
//   bit1
//          0: The data register is not ready for writing
//          1: The data register is ready for writing    
//   bit2
//          0: When all the content of the data register is clear, this bit will be set to 0
//          1: When all the content of the data register is set, this bit will be set to 1
//   bit3-7 
//          Reserved     
#define DEVICE_STATUS_REG (4)

#define STS_READ_ACCESS_BIT    (1<<0)
#define STS_WRITE_ACCESS_BIT   (1<<1)
#define STS_DATA_REGS_OVERFLOW (1<<2)

#define READY         (1)
#define NOT_READY     (0)
#define OVERFLOW      (1)
#define NOT_OVERFLOW  (0)

/****************************STATUS_REGISTER_DEFINITION_END*********************************/


/****************************CONTROL_REGISTER_DEFINITION_START*********************************/

// CONTROL_REGISTER
// Control the read and write ability
// The init value: 0x03
// PAYLOAD
//  bit0:
//       0: be unable to read from data register
//       1: be able to read from data register
//  bit1:
//       0: be unable to write from data register
//       1: be able to write from data register
//  bit2-7:
//       Reserved

#define CONTROL_ACCESS_REG (0)
#define  CTRL_READ_DATA_BIT (1<<0)
#define  CTRL_WRITE_DATA_BIT (1<<1)


#define ENABLE (1)
#define DISABLE (0)
/****************************CONTROL_REGISTER_DEFINITION_END*********************************/
