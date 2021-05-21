public class Pair{

        private String addr;
        private int port;

        public Pair(String _addr, int _port){
            this.addr = _addr;
            this.port = _port;
        }

        public String getAddr(){
            return this.addr;
        }

        public int getPort(){
            return this.port;
        }

}