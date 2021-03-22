

public class Message{
    // Taille des caractéristiques d'un message en octets
    static final int MSG_ID_SIZE = 8;
    static final int MSG_IP_SIZE = 15;
    static final int MSG_MESS_SIZE = 140;
    static final int MSG_NB_MESS = 3;
    static final int MSG_NUM_DIFF_SIZE = 2;
    static final int MSG_NUM_MESS_SIZE = 4;
    static final int MSG_PORT_SIZE = 4;
    static final int MSG_TYPE_SIZE = 4;

    static final String MSG_END = "\r\n";
    static final int MSG_END_SIZE = 2;

    static final char DEFAULT_CHAR = '#'; 
    static final char DEFAULT_INT = '0';

    // Types possibles des messages en octets
    public enum MsgType {
        ACKM,
        DIFF,
        ENDM,
        IMOK,
        ITEM,
        LAST,
        LINB,
        LIST,
        MESS,
        OLDM,
        REGI,
        RENO,
        REOK,
        RUOK
    }

    
}