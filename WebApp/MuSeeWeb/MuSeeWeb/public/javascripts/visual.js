


function flipDoubleBuffer() {
    CUR_BUFFER.hide();
    CUR_BUFFER_IND = 1 - CUR_BUFFER_IND;
    CUR_BUFFER = BUFFERS[CUR_BUFFER_IND];
    CUR_BUFFER.show();
}