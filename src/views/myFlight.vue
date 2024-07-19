<template>
  <div>
    <h3>航班列表</h3>
    <el-button
      type="primary"
      v-if="GLOBAL.hasLogin"
      @click="addFormVisible = true"
      style="margin: 20px"
      >添加航班</el-button
    >
    <el-dialog title="增加航班" :visible.sync="addFormVisible">
      <el-form ref="addForm" :model="addForm" :rules="rules" label-width="80px">
        <el-row :gutter="20">
          <el-col :span="7">
            <el-form-item
              label="出发地"
              prop="departure_city"
              :rules="{ required: true, message: '不能为空', trigger: 'blur' }"
            >
              <el-select
                v-model="addForm.departure_city"
                filterable
                placeholder="请输入"
              >
                <el-option
                  v-for="item in cityLs"
                  :label="item.name"
                  :value="item.id"
                  :key="item.id"
                >
                </el-option>
              </el-select>
            </el-form-item>
          </el-col>
          <el-col :span="7">
            <el-form-item label="目的地" prop="destination_city">
              <el-select
                v-model="addForm.destination_city"
                filterable
                placeholder="请输入"
              >
                <el-option
                  v-for="item in cityLs"
                  :label="item.name"
                  :value="item.id"
                  :key="item.id"
                >
                </el-option>
              </el-select>
            </el-form-item>
          </el-col>
        </el-row>
        <el-form-item label="航班时间" prop="time_range">
          <el-date-picker
            v-model="addForm.time_range"
            type="datetimerange"
            range-separator="--"
            start-placeholder="出发时间"
            end-placeholder="到达时间"
            format="yyyy-MM-dd HH:mm"
            align="right"
          >
          </el-date-picker>
        </el-form-item>
        <el-row :gutter="20">
          <el-col :span="7">
            <el-form-item label="票价" prop="price">
              <el-input v-model="addForm.price" type="number"> </el-input>
            </el-form-item>
          </el-col>
          <el-col :span="7">
            <el-form-item label="里程" prop="mileage">
              <el-input v-model="addForm.mileage" type="number"> </el-input>
            </el-form-item>
          </el-col>
        </el-row>
        <el-form-item>
          <el-button type="primary" @click="submitForm('addForm')"
            >增加</el-button
          >
          <el-button @click="resetForm('addForm')">重置</el-button>
        </el-form-item>
      </el-form>
    </el-dialog>
    <el-form
      ref="filterForm"
      :model="filterForm"
      :rules="rules"
      label-width="80px"
    >
      <el-row>
        <el-col :span="7">
          <el-form-item label="出发地" prop="departure_cityF">
            <el-select
              v-model="filterForm.departure_cityF"
              filterable
              placeholder="请输入"
            >
              <el-option
                v-for="item in cityLs"
                :label="item.name"
                :value="item.id"
                :key="item.id"
              >
              </el-option>
            </el-select>
          </el-form-item>
        </el-col>
        <el-col :span="7">
          <el-form-item label="目的地" prop="destination_cityF">
            <el-select
              v-model="filterForm.destination_cityF"
              filterable
              placeholder="请输入"
            >
              <el-option
                v-for="item in cityLs"
                :label="item.name"
                :value="item.id"
                :key="item.id"
              >
              </el-option>
            </el-select>
          </el-form-item>
        </el-col>
      </el-row>
      <el-form-item label="航班时段" prop="time_rangeF">
        <el-date-picker
          v-model="filterForm.time_rangeF"
          type="datetimerange"
          range-separator="--"
          start-placeholder="最早出发时间"
          end-placeholder="最晚到达时间"
          format="yyyy-MM-dd HH:mm"
          align="right"
        >
        </el-date-picker>
      </el-form-item>
      <el-row :gutter="30">
        <el-col :span="7">
          <el-form-item label="最低票价" prop="low_price">
            <el-input
              v-model="filterForm.low_price"
              placeholder="请输入"
              type="number"
              style="width: 200px"
            ></el-input>
          </el-form-item>
        </el-col>
        <el-col :span="7">
          <el-form-item label="最高票价" prop="high_price">
            <el-input
              v-model="filterForm.high_price"
              placeholder="请输入"
              type="number"
              style="width: 200px"
            ></el-input>
          </el-form-item>
        </el-col>
      </el-row>
      <el-row :gutter="30">
        <el-col :span="7">
          <el-form-item label="排序方式">
            <el-radio-group v-model="filterForm.sortWay">
              <el-radio label="1">按用时排序</el-radio>
              <el-radio label="2">按价格排序</el-radio>
            </el-radio-group>
          </el-form-item>
        </el-col>
        <el-col :span="7">
          <el-form-item>
            <el-button type="primary" @click="submitForm('filterForm')"
              >筛选</el-button
            >
            <el-button @click="resetForm('filterForm')">重置</el-button>
          </el-form-item>
        </el-col>
      </el-row>
    </el-form>
    <aFlight
      :flights="flightLs"
      @flightDeleted="deleteAflight"
      style="margin-bottom: 50px"
    ></aFlight>
  </div>
</template>

<script>
import aFlight from "../components/aFlight";

export default {
  name: "myFlight",
  components: { aFlight },
  data() {
    const priceRule = (rule, value, callback) => {
      if (Number(this.addForm.price) <= 0) {
        callback(new Error("票价须为正数"));
      } else {
        callback();
      }
    };
    const mileageRule = (rule, value, callback) => {
      if (Number(this.addForm.mileage) <= 0) {
        callback(new Error("里程须为正数"));
      } else {
        callback();
      }
    };
    const cityRule = (rule, value, callback) => {
      if (
        this.filterForm.departure_cityF !== "" &&
        this.filterForm.departure_cityF === this.filterForm.destination_cityF
      ) {
        callback(new Error("起点终点不能相同"));
      } else if (
        this.addForm.departure_city !== "" &&
        this.addForm.departure_city === this.addForm.destination_city
      ) {
        callback(new Error("起点终点不能相同"));
      } else {
        callback();
      }
    };
    const timeRule = (rule, value, callback) => {
      const inputTime = new Date(this.addForm.time_range[0]);
      const currentTime = new Date();
      if (inputTime <= currentTime) {
        callback(new Error("出发时间不能早于现在"));
      } else {
        callback();
      }
    };
    const priceFilterRule = (rule, value, callback) => {
      if (
        this.filterForm.low_price > this.filterForm.high_price &&
        this.filterForm.high_price !== ""
      ) {
        let temp = this.filterForm.low_price;
        this.filterForm.low_price = this.filterForm.high_price;
        this.filterForm.high_price = temp;
      }
      if (
        (this.filterForm.low_price !== "" && this.filterForm.low_price <= 0) ||
        (this.filterForm.high_price !== "" && this.filterForm.high_price <= 0)
      ) {
        callback(new Error("价格为正数"));
      } else {
        callback();
      }
    };
    return {
      flightLs: [],
      cityLs: [],
      addFormVisible: false,
      addForm: {
        price: "",
        departure_city: "",
        destination_city: "",
        mileage: "",
        time_range: [],
        depart_time: "",
        arrive_time: "",
      },
      filterForm: {
        departure_cityF: "",
        destination_cityF: "",
        time_rangeF: [],
        earliest_time: "",
        latest_time: "",
        low_price: "",
        high_price: "",
        sortWay: "1",
        ordering: "",
      },
      rules: {
        time_range: [
          { validator: timeRule, trigger: "blur" },
          { required: true, message: "不能为空", trigger: "blur" },
        ],
        destination_city: [
          { validator: cityRule, trigger: "blur" },
          { required: true, message: "不能为空", trigger: "blur" },
        ],
        price: [
          { validator: priceRule, trigger: "blur" },
          { required: true, message: "不能为空", trigger: "blur" },
        ],
        mileage: [
          { validator: mileageRule, trigger: "blur" },
          { required: true, message: "不能为空", trigger: "blur" },
        ],
        destination_cityF: { validator: cityRule, trigger: "blur" },
        low_price: { validator: priceFilterRule, trigger: "blur" },
        high_price: { validator: priceFilterRule, trigger: "blur" },
      },
    };
  },
  mounted() {
    this.$axios({
      method: "get",
      url: "/transportation/flights/",
      headers: {
        "Content-Type": "application/json",
      },
    }).then((res) => {
      //console.log(res.data);
      this.flightLs = res.data;
    });
    this.$axios({
      method: "get",
      url: "/transportation/cities/",
      headers: {
        "Content-Type": "application/json",
      },
    }).then((res) => {
      //console.log(res);
      this.cityLs = res.data;
    });
  },
  methods: {
    submitForm(formName) {
      this.$refs[formName].validate((valid) => {
        if (valid) {
          // alert("o(*￣▽￣*)ブ");
          if (formName === "addForm") {
            this.addForm.depart_time = this.addForm.time_range[0];
            this.addForm.arrive_time = this.addForm.time_range[1];
            this.$axios({
              method: "post",
              url: "/transportation/flights/",
              headers: {
                "Content-Type": "application/json",
                Authorization: `Bearer ${localStorage.getItem("admin_access")}`,
              },
              data: this.addForm,
            })
              .then((res) => {
                console.log(res.data);
                this.flightLs.push(res.data);
                this.addFormVisible = false;
              })
              .catch((error) => {
                console.log(error);
                this.$message.warning("航线里程不应小于城市间距离");
              });
          } else {
            this.filterForm.earliest_time = this.filterForm.time_rangeF[0];
            this.filterForm.latest_time = this.filterForm.time_rangeF[1];
            this.filterForm.ordering =
              this.filterForm.sortWay == 1 ? "time_cost" : "price";
            this.$axios({
              method: "get",
              url: "/transportation/flights/",
              headers: {
                "Content-Type": "application/json",
              },
              params: this.filterForm,
            }).then((res) => {
              this.flightLs = res.data;
            });
          }
          this.resetForm(formName);
        } else {
          // alert("/(ㄒoㄒ)/~~");
          return false;
        }
      });
    },
    resetForm(formName) {
      this.$refs[formName].resetFields();
      this.$axios({
        method: "get",
        url: "/transportation/flights/",
        headers: {
          "Content-Type": "application/json",
        },
      }).then((res) => {
        this.flightLs = res.data;
      });
    },
    deleteAflight() {
      this.$axios({
        method: "get",
        url: "/transportation/flights/",
        headers: {
          "Content-Type": "application/json",
        },
      }).then((res) => {
        this.flightLs = res.data;
        console.log(this.flightLs);
      });
      // console.log("删了(p≧w≦q)");
    },
  },
};
</script>