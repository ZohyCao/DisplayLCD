/*******************************************************************************
 * Copyright:		BUPT
 * File Name:		cmd.c
 * Description:		指令功能实现
 * Author:			ZeroVoid
 * Version:			0.1
 * Data:			2019/09/19 Thu 19:50
 *******************************************************************************/
// TODO: ZeroVoid	due:10/23	模块化组�?
// TODO: ZeroVoid	due:10/7	优化内存分配管理

#include "cmd.h"

#ifdef SL_CMD_DMA

#include "hash.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "GUI_Frame.h"
/* 变量定义 -----------------------------------------------------*/
static const char *delim = ", \r\n\0";
static HashTable cmd_table;

/**
 * @brief	兼�?�V0.1保留变量
 */
UART_HandleTypeDef CMD_USART;
char *cmd_argv[MAX_ARGC]; 
uint8_t DMAaRxBuffer[DMA_BUFFER_SIZE];
char DMAUSART_RX_BUF[DMA_BUFFER_SIZE];
int DMA_RxOK_Flag = 0;
int buffer_count = 0;
char uart_buffer[DMA_BUFFER_SIZE];

/* Private Variables -----------------------------------------------------*/
char cmd_dma_rx_buffer[10];


/* private function -----------------------------------------------------*/
static int str_cmp(const void *a, const void *b);
static void _cmd_help(const void *key, void **value, void *c1);


/* cmd实现函数定义 -----------------------------------------------------*/

void usart_DMA_init(UART_HandleTypeDef *cmd_usart) {
    CMD_USART = *cmd_usart;
    HAL_UART_Receive_DMA(&CMD_USART, (uint8_t *)&DMAaRxBuffer, 99);
    cmd_init();
    __HAL_UART_ENABLE_IT(&CMD_USART,UART_IT_IDLE); // 开�?空闲�?�?
}

void cmd_dma_init(UART_HandleTypeDef *huart) {
    //HAL_UART_Receive_DMA(&huart, )
}

/**
 * @brief	指令初�?�化函数，仅供模块初始化调用
 * @return	None
 */
void cmd_init(void) {
    if (cmd_table == NULL) {
        cmd_table = HashTable_create(str_cmp, hashStr, NULL);
    }
    cmd_add("help", "show cmd usage", cmd_help_func);
}

void usart_exc_DMA() {
    int cmd_argc;
    int erro_n;
    usart_window_update(DMAUSART_RX_BUF);
    erro_n = cmd_parse((char *)DMAUSART_RX_BUF, &cmd_argc, cmd_argv);  //解析命令
    erro_n = cmd_exec(cmd_argc, cmd_argv);                             //执�?�命�?
    UNUSED(erro_n);
    memset(DMAUSART_RX_BUF, 0, 98);
    buffer_count = 0;
}

void HAL_UART_IDLECallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == CMD_USART.Instance) {
        uint8_t temp;
        __HAL_UART_CLEAR_IDLEFLAG(huart);  //清除函数空闲标志
        temp = huart->Instance->SR;
        temp = huart->Instance->DR;  //读出串口的数�?，防止在关闭DMA期间有数�?进来，造成ORE错�??
        UNUSED(temp);
        // temp = hdma_usart3_rx.Instance->CNDTR;
        // huart->hdmarx->XferCpltCallback(huart->hdmarx);
        // //调用DMA接受完毕后的回调函数，最主�?�的�?的是要将串口的状态�?�置为Ready，否则无法开�?下一�?DMA
        HAL_UART_DMAStop(&CMD_USART);  //停�?�本�?DMA

        uint8_t *clr = DMAaRxBuffer-1;
        while(*(++clr) == '\0' && clr < DMAaRxBuffer+DMA_BUFFER_SIZE);
        strcpy((char *)DMAUSART_RX_BUF,(char *)clr);
        if (DMAUSART_RX_BUF[0] != '\0') {
            DMA_RxOK_Flag = 1;
        }
        memset(DMAaRxBuffer, 0, 98);
        HAL_UART_Receive_DMA(&CMD_USART, (uint8_t *)&DMAaRxBuffer, 99);
    }
}

/**
 * @brief	将输入分割，并�?�录参数�?�?
 * @param	cmd_line	输入指令字�?�串
 *          argc        指令�?�?
 *          argv        分割后参数列�?
 * @return	None
 */
int cmd_parse(char *cmd_line,int *argc,char *argv[]){
    char *token = strtok(cmd_line, delim);
    int arg_index = 0;

    while(token && arg_index <= MAX_ARGC) {
        argv[arg_index++] = token;
        token = strtok(NULL, delim);
    }
    *argc = arg_index;
    return 0;
}

/**
 * @brief	指令执�?�函�?
 * @param	argc    参数�?�?
 *          argv    参数列表 
 * @return	0   正常执�?�返�?
 *          1   �?找到指令
 */
int cmd_exec(int argc,char *argv[]){
    struct cmd_info *cmd = (struct cmd_info*)HashTable_get(cmd_table, argv[0]);
    if (cmd != NULL) {
        cmd->cmd_func(argc, argv);
        return 0;
    }
    uprintf("cmd not find\r\n");
    return 1;
}

/**
 * @brief	指令�?助函�?
 * @param	忽略参数
 * @return	None
 */
void cmd_help_func(int argc,char *argv[]){
    uprintf("help:\r\n");
    HashTable_map(cmd_table, _cmd_help, NULL);
}

/**
 * @brief	指令添加函数
 * @param	cmd_name    指令名称
 * @param   cmd_usage   指令使用说明
 * @param   cmd_func    指令函数指针 argc 参数�?�?(�?指令名称), argv 参数字�?�串数组
 * @return	None
 */
void cmd_add(char *cmd_name, char *cmd_usage, void (*cmd_func)(int argc, char *argv[])) {
    // FIXME: ZeroVoid	2019/9/23	 name or usage too long
    struct cmd_info *new_cmd = (struct cmd_info*)malloc(sizeof(struct cmd_info)); 
    char *name = (char*) malloc(strlen(cmd_name)+1);
    char *usage = (char*) malloc(strlen(cmd_usage)+1);
    strcpy(name, cmd_name);
    strcpy(usage, cmd_usage);
    new_cmd->cmd_func = cmd_func;
    new_cmd->cmd_usage = usage;
    HashTable_insert(cmd_table, name, new_cmd);
}

char print_buffer[PRINT_BUFFER_SIZE];
void uprintf(char *fmt, ...) {
    int size;
    va_list arg_ptr;
    va_start(arg_ptr, fmt);
    size = vsnprintf(print_buffer, PRINT_BUFFER_SIZE, fmt, arg_ptr);
    va_end(arg_ptr);

    CMD_USART.gState = HAL_UART_STATE_READY;
    HAL_UART_Transmit_DMA(&CMD_USART, (uint8_t*)print_buffer, size);
    // if (HAL_UART_Transmit_DMA(&CMD_USART, (uint8_t *)print_buffer, size) != HAL_OK) {
    //     HAL_Delay(10);
    // }
    // FIXME: ZeroVoid	2019/10/17	�?速输出或某些情况下会出现卡�?�情�?
    while(CMD_USART.hdmatx->State != HAL_DMA_STATE_READY);
    //HAL_UART_Transmit(&CMD_USART, (uint8_t*)uart_buffer, size, 1000);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
        // 输出完成后，重新设置USART准�?�状态，并解锁串�?,否则无法再�?�输�?
        //huart->gState = HAL_UART_STATE_READY;
        //while(CMD_USART.hdmatx->State != HAL_DMA_STATE_READY);
        // if (huart->hdmatx != NULL) {
        //     huart->hdmatx->State = HAL_DMA_STATE_READY;
        // }
}

void uprintf_to(UART_HandleTypeDef *huart, char *fmt, ...) {
    int size;
    va_list arg_ptr;
    va_start(arg_ptr, fmt);
    size = vsnprintf(print_buffer, PRINT_BUFFER_SIZE, fmt, arg_ptr);
    va_end(arg_ptr);

    //huart->gState = HAL_UART_STATE_READY;
    HAL_UART_Transmit_DMA(huart, (uint8_t *)print_buffer, size);
    //while(huart->hdmatx->State != HAL_DMA_STATE_READY);
    // HAL_UART_Transmit(huart,(uint8_t *)uart_buffer,size,1000);
}

static char s[22] = {'b', 'y', 16, 6};
void send_wave(float arg1, float arg2, float arg3, float arg4) {
    //s[2] = 16;  // length
    //s[3] = 6;   // type
    s[20] = '\r';
    s[21] = '\n';
    memcpy(s + 4, &arg1, sizeof(arg1));
    memcpy(s + 8, &arg2, sizeof(arg1));
    memcpy(s + 12, &arg3, sizeof(arg1));
    memcpy(s + 16, &arg4, sizeof(arg1));
    CMD_USART.gState = HAL_UART_STATE_READY;
    HAL_UART_Transmit_DMA(&CMD_USART, (uint8_t *)s, 22);
    while(CMD_USART.hdmatx->State != HAL_DMA_STATE_READY);
}

/* private function defined -----------------------------------------------------*/
/**
 * @brief	字�?�串比较函数，hash表中使用
 */
static int str_cmp(const void *a, const void *b) {
    return strcmp((char*)a, (char*)b) != 0;
}

/**
 * @brief	输出函数使用说明，遍历hash表中使用
 */
static void _cmd_help(const void *key, void **value, void *c1) {
    UNUSED(c1);
    char *usage = ((struct cmd_info*)(*value))->cmd_usage;
    uprintf("%s: %s\r\n", key, usage);
}

#endif // SL_CMD_DMA