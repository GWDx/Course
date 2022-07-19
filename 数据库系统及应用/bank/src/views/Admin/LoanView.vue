<script setup>
    import Admin from '@/components/Admin.vue'

    // 贷款管理:提供贷款信息的增、删、查功能,提供贷款发放功能;贷款信息一旦添加成
    //      功后不允许修改;要求能查询每笔贷款的当前状态(未开始发放、发放中、已全部发
    //      放);处于发放中状态的贷款记录不允许删除;
</script>

<script>
    import queryServer from '@/utils/queryServer'

    // create table loan
    // (
    //    loanID               numeric(8,0) not null,
    //    allMoney             float(8,2) not null,
    //    primary key (loanID)
    // );

    // create table loanPay
    // (
    //    loanID               numeric(8,0) not null,
    //    payDate              date not null,
    //    payMoney             float(8,2) not null
    // );

    // create table clientLoan
    // (
    //    loanID               numeric(8,0) not null,
    //    clientID             char(20) not null,
    //    primary key (loanID, clientID)
    // );

    queryServer("insert into loan values ('1', '10000')")
    queryServer("insert into loan values ('2', '20000')")
    queryServer("insert into loan values ('3', '30000')")

    queryServer("insert into clientLoan values ('1', '1')")
    queryServer("insert into clientLoan values ('1', '2')")
    queryServer("insert into clientLoan values ('2', '1')")
    queryServer("insert into clientLoan values ('3', '3')")

    queryServer("insert into loanPay values ('1', '2019-01-01', '1000')")
    queryServer("insert into loanPay values ('1', '2019-02-01', '2000')")
    queryServer("insert into loanPay values ('2', '2019-03-01', '1500')")

    export default {
        data() {
            return {
                loanData: [],
                updateLoanID: -1,
                inputLoanID: '',
                inputAllMoney: '',
                inputSendLoan: '',
                inputAllClientID: ''
            }
        },
        methods: {
            queryAllData() {
                // sum loanPay money by loanID, default is 0
                // then join with loan table
                // 'select loanID, 0 from loan' union with 'select loanID, payMoney from loanPay'

                // select * from depositAccount natural join (select accountID, group_concat(clientID) as allClientID from depositClient group by accountID) as temp'
                let command =
                    'select * from loan natural join (select loanID, group_concat(clientID) as allClientID from clientLoan group by loanID) as temp natural join (select loanID, sum(payMoney) as sumPayMoney from loanPay group by loanID union select loanID, 0 from loan where loanID not in (select loanID from loanPay)) as T'
                this.loanData = queryServer(command)
                console.log(this.loanData)
            },
            deleteLoan(index) {
                let sumPayMoney = this.loanData[index].sumPayMoney
                if (0 < sumPayMoney && sumPayMoney < this.loanData[index].allMoney) {
                    alert('该贷款已发放部分金额,不能删除')
                    return
                }
                let loanID = this.loanData[index].loanID
                queryServer(`delete from clientLoan where loanID = '${loanID}'`)
                queryServer(`delete from loanPay where loanID = ${loanID}`)
                queryServer(`delete from loan where loanID = '${loanID}';`)
                this.queryAllData()
            },
            insertLoan() {
                if (!/^\d+$/.test(this.inputLoanID)) {
                    alert('贷款编号必须为数字')
                    return
                }

                let command = `insert into loan values ('${this.inputLoanID}', '${this.inputAllMoney}');`
                queryServer(command)

                let allClientIDList = this.inputAllClientID.split(',')
                for (let item of allClientIDList)
                    queryServer(`insert into clientLoan values ('${this.inputLoanID}', '${item}')`)

                this.updateLoanID = -1
                this.queryAllData()
            },
            moveToInput(index) {
                this.updateLoanID = index
                this.inputLoanID = this.loanData[index].loanID
                this.inputAllMoney = this.loanData[index].allMoney
                this.inputAllClientID = this.loanData[index].allClientID
            },
            sendLoan(index) {
                let loanID = this.loanData[index].loanID
                let today = new Date()
                let todayString = today.getFullYear() + '-' + (today.getMonth() + 1) + '-' + today.getDate()
                let command = `insert into loanPay values ('${loanID}', '${todayString}', '${this.inputSendLoan}');`
                queryServer(command)
                this.queryAllData()
            },
            queryInputData() {
                // 模糊查询
                let command = `select * from (select * from loan natural join (select loanID, group_concat(clientID) as allClientID from clientLoan group by loanID) as temp natural join (select loanID, sum(payMoney) as sumPayMoney from loanPay group by loanID union select loanID, 0 from loan where loanID not in (select loanID from loanPay)) as T) as T2 where loanID like '%${this.inputLoanID}%' and allMoney like '%${this.inputAllMoney}%';`
                this.loanData = queryServer(command)
            }
        }
    }
</script>

<template>
    <Admin highlight="2" />

    <div>
        <button @click="queryInputData" class="queryButton">查询数据</button>
    </div>

    <div class="loanTable">
        <table>
            <thead>
                <tr>
                    <th>贷款ID</th>
                    <th>贷款总金额</th>
                    <th>总发放金额</th>
                    <th>关联客户</th>
                    <th>插入删除</th>
                    <th>复制</th>
                    <th>发放</th>
                </tr>
            </thead>
            <tbody>
                <tr>
                    <td>
                        <input class="insertInput" v-model="inputLoanID" type="text" />
                    </td>
                    <td>
                        <input class="insertInput" v-model="inputAllMoney" type="text" />
                    </td>
                    <td></td>
                    <td>
                        <input class="insertInput" v-model="inputAllClientID" type="text" />
                    </td>
                    <td>
                        <button @click="insertLoan" class="insert-button">插入</button>
                    </td>
                    <td></td>
                    <td>
                        <input class="insertInput" v-model="inputSendLoan" type="text" />
                    </td>
                </tr>

                <tr v-for="(item, index) in loanData">
                    <td>{{ item.loanID }}</td>
                    <td>{{ item.allMoney }}</td>
                    <td>{{ item.sumPayMoney }}</td>
                    <td>{{ item.allClientID }}</td>
                    <td>
                        <button @click="deleteLoan(index)" class="update-button">删除</button>
                    </td>
                    <td>
                        <button @click="moveToInput(index)" class="update-button">复制</button>
                    </td>
                    <td>
                        <button @click="sendLoan(index)" class="update-button">发放</button>
                    </td>
                </tr>
            </tbody>
        </table>
    </div>
</template>

<style src="@/assets/queryTableView.css"></style>
