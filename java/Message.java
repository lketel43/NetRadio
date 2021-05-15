import java.util.Arrays;

public class Message{
    // Taille des caracteristiques d'un message en octets
    static final int MSG_ID_SIZE = 8;
    static final int MSG_IP_SIZE = 15;
    static final int MSG_MESS_SIZE = 140;
    static final int MSG_NB_MESS = 3;
    static final int MSG_NUM_DIFF_SIZE = 2;
    static final int MSG_NUM_MESS_SIZE = 4;
    static final int MSG_PORT_SIZE = 4;
    static final int MSG_TYPE_SIZE = 4;

    static final int MSG_END_SIZE = 2;
    static final byte MSG_R = (byte) '\r';
    static final byte MSG_N = (byte) '\n';
    static final byte[] MSG_END = {MSG_R, MSG_N};

    static final byte[] BYTE_SPACE = {(byte) ' '};

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
        RUOK,
        IMAG
    }

    /* */

    private static String removeDefChar(String str){
        int index = -1;
        for(int i = 0; i < str.length(); i++){
            if(str.charAt(i) == '#'){
                index = i; 
                break;
            }
        }
        if(index == -1) return str;
        else return str.substring(0, index);
    }

    public static MsgType getType(byte[] msg){
        String type = new String(java.util.Arrays.copyOfRange(msg, 0, MSG_TYPE_SIZE));
        switch(type){
            case "ACKM" : return MsgType.ACKM;
            case "DIFF" : return MsgType.DIFF;
            case "ENDM" : return MsgType.ENDM;
            case "IMOK" : return MsgType.IMOK;
            case "ITEM" : return MsgType.ITEM;
            case "LAST" : return MsgType.LAST;
            case "LINB" : return MsgType.LINB;
            case "LIST" : return MsgType.LIST;
            case "MESS" : return MsgType.MESS;
            case "OLDM" : return MsgType.OLDM;
            case "REGI" : return MsgType.REGI;
            case "RENO" : return MsgType.RENO;
            case "REOK" : return MsgType.REOK;
            case "RUOK" : return MsgType.RUOK;
            case "IMAG" : return MsgType.IMAG;
            default : throw new IllegalArgumentException("Wrong message format");
        }
    }

    public static String getID(byte[] msg){
        MsgType type = getType(msg);
        switch(type){
            case OLDM : case DIFF : 
                return removeDefChar(new String(java.util.Arrays.copyOfRange(msg, MSG_TYPE_SIZE + 1 + MSG_NUM_MESS_SIZE + 1,
                                                                    MSG_TYPE_SIZE + 1 + MSG_NUM_MESS_SIZE + 1 + MSG_ID_SIZE)));
            case MESS : case REGI : case ITEM : 
                return removeDefChar(new String(java.util.Arrays.copyOfRange(msg, MSG_TYPE_SIZE + 1, MSG_TYPE_SIZE + 1 + MSG_ID_SIZE)));
            default : throw new IllegalArgumentException("Wrong message format");
        }
    }

    public static String getMsg(byte[] msg){
        MsgType type = getType(msg);
        switch(type){
            case MESS : return removeDefChar(new String(java.util.Arrays.copyOfRange(msg, MSG_TYPE_SIZE + 1 + MSG_ID_SIZE + 1,
                                                                            MSG_TYPE_SIZE + 1 + MSG_ID_SIZE + 1 + MSG_MESS_SIZE)));
            case DIFF : case OLDM : return removeDefChar(new String(java.util.Arrays.copyOfRange(msg, MSG_TYPE_SIZE + 1 + MSG_NUM_MESS_SIZE + 1 + MSG_ID_SIZE + 1,
                                                                                        MSG_TYPE_SIZE + 1 + MSG_NUM_MESS_SIZE + 1 + MSG_ID_SIZE + 1 + MSG_MESS_SIZE)));
            default : throw new IllegalArgumentException("Wrong message format");
        }
    }

    public static String getNumMsg(byte[] msg){
        MsgType type = getType(msg);
        switch(type){
            case DIFF : case OLDM :
                return new String(java.util.Arrays.copyOfRange(msg, MSG_TYPE_SIZE + 1, MSG_TYPE_SIZE + 1 + MSG_NUM_MESS_SIZE));
            default : throw new IllegalArgumentException("Wrong message format");
        }
    }

    public static int getNbMsg(byte[] msg){
        if(getType(msg) == MsgType.LAST) return Integer.parseInt(new String(java.util.Arrays.copyOfRange(msg, MSG_TYPE_SIZE + 1, MSG_TYPE_SIZE + 1 + MSG_NB_MESS)));
        throw new IllegalArgumentException("Wrong message format");
    }

    public static String getIp1(byte[] msg){
        MsgType type = getType(msg);
        if(type == MsgType.REGI || type == MsgType.ITEM) return new String(java.util.Arrays.copyOfRange(msg, MSG_TYPE_SIZE + 1 + MSG_ID_SIZE + 1, 
                                                                                                             MSG_TYPE_SIZE + 1 + MSG_ID_SIZE + 1 + MSG_IP_SIZE));
        throw new IllegalArgumentException("Wrong message format");
    }

    public static String getIp2(byte[] msg){
        MsgType type = getType(msg);
        if(type == MsgType.REGI || type == MsgType.ITEM) return new String(java.util.Arrays.copyOfRange(msg, MSG_TYPE_SIZE + 1 + MSG_ID_SIZE + 1 + MSG_IP_SIZE + 1 + MSG_PORT_SIZE + 1, 
                                                                                                             MSG_TYPE_SIZE + 1 + MSG_ID_SIZE + 1 + MSG_IP_SIZE + 1 + MSG_PORT_SIZE + 1 + MSG_IP_SIZE));
        throw new IllegalArgumentException("Wrong message format");
    }

    public static int getPort1(byte[] msg){
        MsgType type = getType(msg);
        if(type == MsgType.REGI || type == MsgType.ITEM) return Integer.parseInt(new String(java.util.Arrays.copyOfRange(msg, MSG_TYPE_SIZE + 1 + MSG_ID_SIZE + 1 + MSG_IP_SIZE + 1, 
                                                                                                                              MSG_TYPE_SIZE + 1 + MSG_ID_SIZE + 1 + MSG_IP_SIZE + 1 + MSG_PORT_SIZE)));
        throw new IllegalArgumentException("Wrong message format");
    }

    public static int getPort2(byte[] msg){
        MsgType type = getType(msg);
        if(type == MsgType.REGI || type == MsgType.ITEM) return Integer.parseInt(new String(java.util.Arrays.copyOfRange(msg, MSG_TYPE_SIZE + 1 + MSG_ID_SIZE + 1 + MSG_IP_SIZE + 1 + MSG_PORT_SIZE + 1 + MSG_IP_SIZE + 1,
                                                                                                                              MSG_TYPE_SIZE + 1 + MSG_ID_SIZE + 1 + MSG_IP_SIZE + 1 + MSG_PORT_SIZE + 1 + MSG_IP_SIZE + 1 + MSG_PORT_SIZE)));
        throw new IllegalArgumentException("Wrong message format");
    }

    public static int getNumDiff(byte[] msg){
        if(getType(msg) == MsgType.LINB) return Integer.parseInt(new String(java.util.Arrays.copyOfRange(msg, MSG_TYPE_SIZE + 1, 
                                                                                                              MSG_TYPE_SIZE + 1 + MSG_NUM_DIFF_SIZE)));
        throw new IllegalArgumentException("Wrong message format");
    }


    /*Methods to create messages*/

    private static byte[] intToCharBytes(String str, int nbBytes){
        byte[] bytes = new byte[nbBytes];
        Arrays.fill(bytes, (byte) DEFAULT_INT);
        byte[] tmpBytes = str.getBytes();
        int index = nbBytes - tmpBytes.length;
        for(int i = index; i < nbBytes; i++) bytes[i] = tmpBytes[i - index];
        return bytes;
    }

    private static byte[] msgToBytes(String msg){
        byte[] bytesMsg = new byte[MSG_MESS_SIZE];
        byte[] tmpBytesMsg = msg.getBytes();
        for(int i = 0; i < MSG_MESS_SIZE; i++){
            if(i < tmpBytesMsg.length) bytesMsg[i] = tmpBytesMsg[i];
            else bytesMsg[i] = (byte) DEFAULT_CHAR;
        }
        return bytesMsg;
    }

    private static String getMsgFromString(String mess){
        String[] fields = mess.split(" ");
        switch(fields[0]){
            case "OLDM" : case "DIFF" :
                int index = MSG_TYPE_SIZE + fields[1].length() + fields[2].length() + 3;
                return mess.substring(index, mess.length());
            case "MESS" : 
                int index2 = MSG_TYPE_SIZE + fields[1].length() + 2;
                return mess.substring(index2, mess.length());
            default : throw new IllegalArgumentException("Wrong message format");
        }
    }

    
    private static byte[] ipToBytes(String ip){
        byte[] bytes = ip.getBytes();
        if(bytes.length < 15){
            byte[] allBytes = new byte[MSG_IP_SIZE];
            Arrays.fill(allBytes, (byte) DEFAULT_INT);
            allBytes[3] = (byte) '.';
            allBytes[7] = (byte) '.';
            allBytes[11] = (byte) '.';
            String[] fields = ip.split("\\.");
            for(int i = 0; i < fields.length; i++){
                int index = 3 - fields[i].length();
                for(int j = index; j < index + fields[i].length(); j++){
                    allBytes[(4 * i) + j] = (byte) fields[i].charAt(j - index);
                }
            }
            return allBytes;
        }
        return bytes;
    }

    private static byte[] typeToBytes(String type){
        return type.getBytes();
    }

    private static byte[] idToBytes(String id){
        byte[] idBytes = new byte[MSG_ID_SIZE];
        Arrays.fill(idBytes, (byte) DEFAULT_CHAR);
        for(int i = 0; i < id.length(); i++) idBytes[i] = (byte) id.charAt(i);
        return idBytes;
    }

    private static byte[] concatByteArray(byte[] ...args){
        int size = 0;
        for(byte[] arr : args) size += arr.length;
        byte[] ret = new byte[size];
        int index = 0;
        for(byte[] arr : args){
            for(int i = 0; i < arr.length; i++) ret[i + index] = arr[i];
            index += arr.length;
        }
        return ret;
    }

    private static byte[] createMsgFromBytes(byte[] ...args){
        if(args.length == 0) return null;
        else return concatByteArray(args);
    }

    public static byte[] createMsg(String msg){
        String[] fields = msg.split(" ");
        switch(fields[0]){
            case "IMOK" : case "RUOK" : case "LIST" :
            case "RENO" : case "REOK" : case "ACKM" :
            case "ENDM" :
                byte[] t = typeToBytes(fields[0]);
                byte[][] b = {t, MSG_END};
                return createMsgFromBytes(b);
            case "REGI" : case "ITEM":
                byte[] type = typeToBytes(fields[0]);
                byte[] id = idToBytes(fields[1]);
                byte[] ip1 = ipToBytes(fields[2]);
                byte[] port1 = intToCharBytes(fields[3], MSG_PORT_SIZE);
                byte[] ip2 = ipToBytes(fields[4]);
                byte[] port2 = intToCharBytes(fields[5], MSG_PORT_SIZE);
                byte[][] bytes = {type, BYTE_SPACE, id, BYTE_SPACE, ip1, BYTE_SPACE, port1, BYTE_SPACE, ip2, BYTE_SPACE, port2, MSG_END};
                return createMsgFromBytes(bytes);
            case "OLDM" : case "DIFF" :
                byte[] type2 = typeToBytes(fields[0]);
                byte[] numMsg = intToCharBytes(fields[1], MSG_NUM_MESS_SIZE);
                byte[] id2 = idToBytes(fields[2]);
                String strMsg = getMsgFromString(msg);
                byte[] mess = msgToBytes(strMsg);
                byte[][] bytes2 = {type2, BYTE_SPACE, numMsg, BYTE_SPACE, id2, BYTE_SPACE, mess, MSG_END};
                return createMsgFromBytes(bytes2); 
            case "LAST" :
                byte[] type3 = typeToBytes(fields[0]);
                byte[] nbMsg = intToCharBytes(fields[1], MSG_NB_MESS);
                byte[][] bytes3 = {type3, BYTE_SPACE, nbMsg, MSG_END};
                return createMsgFromBytes(bytes3);
            case "MESS" :
                byte[] type4 = typeToBytes(fields[0]);
                byte[] id3 = idToBytes(fields[1]);
                String strMsg2 = getMsgFromString(msg);
                byte[] mess2 = msgToBytes(strMsg2);
                byte[][] bytes4 = {type4, BYTE_SPACE, id3, BYTE_SPACE, mess2, MSG_END};
                return createMsgFromBytes(bytes4);
            case "LINB" :
                byte[] type5 = typeToBytes(fields[0]);
                byte[] numDiff = intToCharBytes(fields[1], MSG_NUM_DIFF_SIZE);
                byte[][] bytes5 = {type5, BYTE_SPACE, numDiff, MSG_END};
                return createMsgFromBytes(bytes5);
            default : throw new IllegalArgumentException("Wrong message format");

        }
    }

    public static byte[] DiffToOldm(byte[] mess){
        String type = "OLDM";
        String numMess = getNumMsg(mess);
        String id = getID(mess);
        String msg = getMsg(mess);
        String newMess = type + " " + numMess + " " + id + " " + msg;
        return createMsg(newMess);
    }
}