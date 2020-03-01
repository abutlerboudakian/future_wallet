#include <stdio.h>
#include <SQLAPI.h>

int main(int argc, char* argv[]) {
	SAConnection con;

	if(argc != 3) {
		printf("Incorrect number of arguments, exiting program.");
	} else {
		try {
			con.setOption(_TSA("UseAPI")) = _TSA("OLEDB");
			con.setClient(SA_SQLServer_Client);
			con.Connect("74.70.80.116", argv[1], argv[2]);

			SACommand cmd(&con, "SELECT TOP(10) Open FROM modeldata.Stocks ORDER BY Timestamp DESC;");
			cmd.execute();

			while(cmd.FetchNext()) {
				double open = select[1];
				cout << "Open Price: $" << open;
			}

			con.Disconnect();
		} catch(SAException &x) {
			con.Rollback();
		}
	}

	return 0;
}